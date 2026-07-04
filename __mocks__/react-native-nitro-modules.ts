/**
 * Jest mock of the Nitro layer. The real `createHybridObject` binds to a native
 * HybridObject and throws in plain node, so `import ... from '../index'` would
 * blow up at module load. This returns a Proxy that lazily makes a `jest.fn`
 * per accessed method, letting tests assert on InspireFace.launch,
 * ModelManager.downloadModel, etc. Native behaviour (session lifetime, threads)
 * is NOT covered here — that's the stress harness (`npm run test:stress`).
 */
const registry: Record<string, Record<string, jest.Mock>> = {};
// Track every lazily-created fn so __resetHybrids can reset them IN PLACE —
// recreating stubs would orphan the references index.tsx captured at load.
const createdFns: jest.Mock[] = [];

function makeHybridStub(name: string): Record<string, jest.Mock> {
  const store: Record<string, jest.Mock> = {};
  return new Proxy(store, {
    get(target, prop: string) {
      if (prop === '__name') return name as never;
      if (!target[prop]) {
        target[prop] = jest.fn();
        createdFns.push(target[prop]);
      }
      return target[prop];
    },
  });
}

export const NitroModules = {
  createHybridObject: jest.fn((name: string) => {
    if (!registry[name]) registry[name] = makeHybridStub(name);
    return registry[name];
  }),
  box: jest.fn((o: unknown) => o),
};

/**
 * Grab the same stub instance the module under test received. Pass the expected
 * method shape so property access is typed (the underlying Proxy lazily makes a
 * jest.fn for any accessed name regardless of T).
 */
export function __getHybrid<T = Record<string, jest.Mock>>(name: string): T {
  if (!registry[name]) registry[name] = makeHybridStub(name);
  return registry[name] as unknown as T;
}

/** Reset stub call history + return values between tests (same instances). */
export function __resetHybrids(): void {
  for (const fn of createdFns) fn.mockReset();
}
