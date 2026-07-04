import { readFileSync } from 'fs';
import { join } from 'path';

/**
 * Static regression guard on the SHIPPED C++. These tests run in plain node (no
 * device, no build) and fail the instant someone removes a lock, a null-check,
 * or a `withNativeHandle` borrow from the native wrappers — the invariants that
 * prevent the use-during-dispose crash (EXC_BAD_ACCESS in MNN) we fixed.
 *
 * They parse the real .cpp/.hpp text, so they can't drift from what ships. The
 * deterministic runtime proof lives in the stress harness (`npm run test:stress`);
 * these are the cheap tripwire that gates every PR even when the sim job doesn't.
 */

const cppDir = join(__dirname, '..', '..', 'cpp');
const read = (f: string) => readFileSync(join(cppDir, f), 'utf8');

type Method = { name: string; body: string };

// Split a .cpp into { name, body } per `Ret Class::method(...) {` definition.
function methodsOf(src: string, klass: string): Method[] {
  const re = new RegExp(
    `${klass}::(~?\\w+)\\s*\\([^)]*\\)\\s*(?:const\\s*)?(?:noexcept\\s*)?\\{`,
    'g'
  );
  const starts: { name: string; idx: number }[] = [];
  let m: RegExpExecArray | null;
  while ((m = re.exec(src)) !== null) {
    const name = m[1];
    if (name !== undefined) starts.push({ name, idx: m.index });
  }
  return starts.map((s, i) => ({
    name: s.name,
    body: src.slice(s.idx, starts[i + 1]?.idx ?? src.length),
  }));
}

// Look up a method that must exist, or fail the test with a clear message.
function requireMethod(methods: Method[], name: string): Method {
  const fn = methods.find((x) => x.name === name);
  if (fn === undefined) throw new Error(`method ${name} not found (renamed?)`);
  return fn;
}

describe('HybridSession locking (dispose-race regression guard)', () => {
  const src = read('HybridSession.cpp');
  const methods = methodsOf(src, 'HybridSession');
  // ctor/dtor/getExternalMemorySize handled separately; the rest must lock.
  const exempt = new Set([
    'HybridSession',
    '~HybridSession',
    'getExternalMemorySize',
  ]);

  it('every method touching _session takes the mutex', () => {
    const offenders: string[] = [];
    for (const fn of methods) {
      if (exempt.has(fn.name)) continue;
      if (!/_session/.test(fn.body)) continue;
      if (
        !/std::lock_guard<std::mutex>\s+lock\(_sessionMutex\)/.test(fn.body)
      ) {
        offenders.push(fn.name);
      }
    }
    expect(offenders).toEqual([]);
  });

  it('cleanup locks BEFORE releasing the native session', () => {
    const cleanup = requireMethod(methods, 'cleanup');
    expect(cleanup.body).toMatch(
      /std::lock_guard<std::mutex>\s+lock\(_sessionMutex\)/
    );
    expect(cleanup.body.indexOf('lock(_sessionMutex)')).toBeLessThan(
      cleanup.body.indexOf('HFReleaseInspireFaceSession')
    );
  });

  it('extractFaceFeature has the lock + null-check its siblings have (was missing)', () => {
    const fn = requireMethod(methods, 'extractFaceFeature');
    expect(fn.body).toMatch(
      /std::lock_guard<std::mutex>\s+lock\(_sessionMutex\)/
    );
    expect(fn.body).toMatch(/_session\s*==\s*nullptr/);
  });

  it('the session mutex is a plain exclusive std::mutex (writers, not shared)', () => {
    const hpp = read('HybridSession.hpp');
    expect(hpp).toMatch(/mutable std::mutex _sessionMutex;/);
    expect(hpp).not.toMatch(/shared_mutex/);
  });

  it('stream handles are borrowed via withNativeHandle, never a bare getter', () => {
    expect(src).not.toMatch(/getNativeHandle\(\)/);
    expect(src).toMatch(/withNativeHandle/);
  });
});

describe.each([
  ['HybridImageStream', '_stream', 'HFReleaseImageStream'],
  ['HybridImageBitmap', '_bitmap', 'HFReleaseImageBitmap'],
])('%s locking', (klass, handle, releaseFn) => {
  const src = read(`${klass}.cpp`);
  const hpp = read(`${klass}.hpp`);
  const methods = methodsOf(src, klass);
  const exempt = new Set([klass, `~${klass}`, 'getExternalMemorySize']);

  it('has a mutex member and a withNativeHandle borrow', () => {
    expect(hpp).toMatch(/mutable std::mutex _mutex;/);
    expect(hpp).toMatch(/withNativeHandle/);
  });

  it(`every method touching ${handle} locks _mutex`, () => {
    const offenders: string[] = [];
    for (const fn of methods) {
      if (exempt.has(fn.name)) continue;
      if (!new RegExp(handle).test(fn.body)) continue;
      if (!/std::lock_guard<std::mutex>\s+lock\(_mutex\)/.test(fn.body))
        offenders.push(fn.name);
    }
    expect(offenders).toEqual([]);
  });

  it(`cleanup locks before ${releaseFn}`, () => {
    const cleanup = requireMethod(methods, 'cleanup');
    expect(cleanup.body).toMatch(
      /std::lock_guard<std::mutex>\s+lock\(_mutex\)/
    );
    expect(cleanup.body.indexOf('lock(_mutex)')).toBeLessThan(
      cleanup.body.indexOf(releaseFn)
    );
  });

  it('getExternalMemorySize is non-blocking (try_to_lock) and dispose-aware', () => {
    const fn = requireMethod(methods, 'getExternalMemorySize');
    expect(fn.body).toMatch(/std::try_to_lock/);
    expect(fn.body).toMatch(new RegExp(`${handle}\\s*(!=|==)\\s*nullptr`));
  });
});

describe('HybridInspireFace FeatureHub + global-state locking', () => {
  const src = read('HybridInspireFace.cpp');
  const methods = methodsOf(src, 'HybridInspireFace');

  it('every featureHub* method takes the global hub mutex', () => {
    const offenders: string[] = [];
    for (const fn of methods) {
      if (!/^featureHub/.test(fn.name)) continue;
      if (
        !/std::lock_guard<std::mutex>\s+hubLock\(gFeatureHubMutex\)/.test(
          fn.body
        )
      ) {
        offenders.push(fn.name);
      }
    }
    expect(offenders).toEqual([]);
  });

  it('terminate and reload take the hub mutex (global teardown vs hub race)', () => {
    for (const name of ['terminate', 'reload']) {
      const fn = requireMethod(methods, name);
      expect(fn.body).toMatch(
        /std::lock_guard<std::mutex>\s+hubLock\(gFeatureHubMutex\)/
      );
    }
  });

  it('featureHubDataEnable does not hand-manage the path buffer with new[]/delete[]', () => {
    const fn = requireMethod(methods, 'featureHubDataEnable');
    expect(fn.body).not.toMatch(/new char\[/);
    expect(fn.body).not.toMatch(/delete\[\]/);
  });
});
