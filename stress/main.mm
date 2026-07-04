// Headless stress-test harness for the InspireFace C API race condition
// seen in production: EXC_BAD_ACCESS in MNN::Interpreter::getSessionOutput
// when HFExecuteFaceTrack races HFReleaseInspireFaceSession.
//
// Subcommands:
//   repro <modelPath> <imagePath> [cycles]   - unguarded race; a crash here is SUCCESS
//   fixed <modelPath> <imagePath> [cycles]   - same workload guarded by std::shared_mutex; must exit 0
//   hub <modelPath> [seconds]                - FeatureHub insert/remove vs search race, unguarded
//   hubfixed <modelPath> [seconds]           - same guarded by a single std::mutex; must exit 0
//
// Build: see build.sh. Run inside the iOS simulator via `xcrun simctl spawn booted ...`.

#import <Foundation/Foundation.h>
#import <ImageIO/ImageIO.h>
#import <CoreGraphics/CoreGraphics.h>

#include <inspireface.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

static void logf_(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  fflush(stderr);
}

// ---------------------------------------------------------------------------
// Image loading
// ---------------------------------------------------------------------------

// Preferred path: let InspireFace decode the file itself.
static HFImageStream createStreamViaInspireFace(const char *imagePath) {
  HFImageBitmap bitmap = nullptr;
  HResult r = HFCreateImageBitmapFromFilePath(imagePath, 3, &bitmap);
  if (r != HSUCCEED || bitmap == nullptr) {
    logf_("[setup] HFCreateImageBitmapFromFilePath failed (code=%ld), will fall back to ImageIO", (long)r);
    return nullptr;
  }
  HFImageStream stream = nullptr;
  r = HFCreateImageStreamFromImageBitmap(bitmap, HF_CAMERA_ROTATION_0, &stream);
  if (r != HSUCCEED || stream == nullptr) {
    logf_("[setup] HFCreateImageStreamFromImageBitmap failed (code=%ld)", (long)r);
    HFReleaseImageBitmap(bitmap);
    return nullptr;
  }
  // NOTE: intentionally NOT releasing the bitmap here; the stream may reference
  // its pixels and this harness keeps the stream alive for the whole run anyway.
  return stream;
}

// Fallback: decode with ImageIO/CoreGraphics into a BGR buffer.
static std::vector<uint8_t> g_fallbackPixels; // must outlive the stream
static HFImageStream createStreamViaImageIO(const char *imagePath) {
  NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:imagePath]];
  CGImageSourceRef src = CGImageSourceCreateWithURL((__bridge CFURLRef)url, NULL);
  if (!src) { logf_("[setup] CGImageSourceCreateWithURL failed"); return nullptr; }
  CGImageRef img = CGImageSourceCreateImageAtIndex(src, 0, NULL);
  CFRelease(src);
  if (!img) { logf_("[setup] CGImageSourceCreateImageAtIndex failed"); return nullptr; }

  const size_t w = CGImageGetWidth(img);
  const size_t h = CGImageGetHeight(img);
  std::vector<uint8_t> rgba(w * h * 4);
  CGColorSpaceRef cs = CGColorSpaceCreateDeviceRGB();
  CGContextRef ctx = CGBitmapContextCreate(rgba.data(), w, h, 8, w * 4, cs,
                                           kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
  CGColorSpaceRelease(cs);
  if (!ctx) { CGImageRelease(img); logf_("[setup] CGBitmapContextCreate failed"); return nullptr; }
  CGContextDrawImage(ctx, CGRectMake(0, 0, w, h), img);
  CGContextRelease(ctx);
  CGImageRelease(img);

  g_fallbackPixels.resize(w * h * 3);
  for (size_t i = 0; i < w * h; i++) { // RGBA -> BGR
    g_fallbackPixels[i * 3 + 0] = rgba[i * 4 + 2];
    g_fallbackPixels[i * 3 + 1] = rgba[i * 4 + 1];
    g_fallbackPixels[i * 3 + 2] = rgba[i * 4 + 0];
  }
  HFImageData data{};
  data.data = g_fallbackPixels.data();
  data.width = (HInt32)w;
  data.height = (HInt32)h;
  data.format = HF_STREAM_BGR;
  data.rotation = HF_CAMERA_ROTATION_0;
  HFImageStream stream = nullptr;
  HResult r = HFCreateImageStream(&data, &stream);
  if (r != HSUCCEED) { logf_("[setup] HFCreateImageStream failed (code=%ld)", (long)r); return nullptr; }
  return stream;
}

static HFImageStream loadImageStream(const char *imagePath) {
  HFImageStream s = createStreamViaInspireFace(imagePath);
  if (!s) s = createStreamViaImageIO(imagePath);
  return s;
}

// ---------------------------------------------------------------------------
// Common setup
// ---------------------------------------------------------------------------

static void launchSDK(const char *modelPath) {
  HResult r = HFLaunchInspireFace(modelPath);
  if (r != HSUCCEED) {
    logf_("[setup] HFLaunchInspireFace('%s') failed: %ld", modelPath, (long)r);
    exit(2);
  }
  logf_("[setup] SDK launched with pack: %s", modelPath);
}

// Mirrors the app: recognition + quality, LIGHT_TRACK, max 1 face.
static HFSession createSession() {
  HOption opt = HF_ENABLE_FACE_RECOGNITION | HF_ENABLE_QUALITY;
  HFSession session = nullptr;
  HResult r = HFCreateInspireFaceSessionOptional(opt, HF_DETECT_MODE_LIGHT_TRACK,
                                                 /*maxDetectFaceNum=*/1,
                                                 /*detectPixelLevel=*/-1,
                                                 /*trackByDetectModeFPS=*/-1, &session);
  if (r != HSUCCEED || session == nullptr) {
    logf_("[setup] HFCreateInspireFaceSessionOptional failed: %ld", (long)r);
    exit(2);
  }
  return session;
}

// ---------------------------------------------------------------------------
// repro / fixed: use-during-free race on HFSession
// ---------------------------------------------------------------------------

struct RaceResult {
  long tracksBeforeRelease = 0;
  long tracksAfterRelease = 0;
  long errorsAfterRelease = 0;
};

// One create/track/release cycle WITHOUT any locking (production bug shape).
// Returns stats; if the SDK use-after-free fires, the process dies here.
static RaceResult runUnguardedCycle(HFImageStream stream, std::mt19937 &rng) {
  HFSession session = createSession();

  std::atomic<bool> stop{false};
  std::atomic<bool> released{false};
  std::atomic<long> tracksBefore{0}, tracksAfter{0}, errsAfter{0};
  std::atomic<bool> inFlight{false};

  std::thread tracker([&]() {
    while (!stop.load(std::memory_order_acquire)) {
      HFMultipleFaceData results{};
      inFlight.store(true, std::memory_order_release);
      HResult r = HFExecuteFaceTrack(session, stream, &results); // <-- races the release below
      inFlight.store(false, std::memory_order_release);
      if (released.load(std::memory_order_acquire)) {
        tracksAfter.fetch_add(1);
        if (r != HSUCCEED) errsAfter.fetch_add(1);
      } else {
        tracksBefore.fetch_add(1);
      }
    }
  });

  // Hostile timing: wait until a track call is actually in flight, then add a
  // small random delay so the release lands at a random point inside/around
  // HFExecuteFaceTrack (which internally walks MNN interpreter sessions).
  while (!inFlight.load(std::memory_order_acquire)) {
    std::this_thread::yield();
  }
  std::uniform_int_distribution<int> ms(20, 200);
  std::this_thread::sleep_for(std::chrono::milliseconds(ms(rng)));

  released.store(true, std::memory_order_release);
  HResult rr = HFReleaseInspireFaceSession(session); // <-- frees MNN interpreters under the tracker
  (void)rr;

  // Let the tracker keep hammering the freed session briefly.
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  stop.store(true, std::memory_order_release);
  tracker.join();

  RaceResult res;
  res.tracksBeforeRelease = tracksBefore.load();
  res.tracksAfterRelease = tracksAfter.load();
  res.errorsAfterRelease = errsAfter.load();
  return res;
}

// Same workload guarded exactly like the planned HybridSession fix:
// shared_lock in method bodies, unique_lock in cleanup.
struct GuardedSession {
  std::shared_mutex mutex;
  HFSession session = nullptr;
};

static RaceResult runGuardedCycle(HFImageStream stream, std::mt19937 &rng) {
  GuardedSession g;
  g.session = createSession();

  std::atomic<bool> stop{false};
  std::atomic<long> tracksBefore{0}, tracksAfterNull{0};

  std::thread tracker([&]() {
    while (!stop.load(std::memory_order_acquire)) {
      HFMultipleFaceData results{};
      {
        std::shared_lock<std::shared_mutex> lock(g.mutex); // <-- planned fix pattern
        if (g.session == nullptr) {
          tracksAfterNull.fetch_add(1);
          continue;
        }
        HResult r = HFExecuteFaceTrack(g.session, stream, &results);
        (void)r;
        tracksBefore.fetch_add(1);
      }
    }
  });

  std::uniform_int_distribution<int> ms(20, 200);
  std::this_thread::sleep_for(std::chrono::milliseconds(ms(rng)));

  {
    std::unique_lock<std::shared_mutex> lock(g.mutex); // <-- exclusive lock in cleanup
    HFReleaseInspireFaceSession(g.session);
    g.session = nullptr;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  stop.store(true, std::memory_order_release);
  tracker.join();

  RaceResult res;
  res.tracksBeforeRelease = tracksBefore.load();
  res.tracksAfterRelease = tracksAfterNull.load();
  return res;
}

// Measure avg HFExecuteFaceTrack latency with and without the shared lock.
static void measureLockOverhead(HFImageStream stream) {
  const int N = 60;
  HFSession session = createSession();
  HFMultipleFaceData results{};

  // Warmup (first calls include lazy MNN allocations)
  for (int i = 0; i < 5; i++) HFExecuteFaceTrack(session, stream, &results);

  auto t0 = std::chrono::steady_clock::now();
  for (int i = 0; i < N; i++) HFExecuteFaceTrack(session, stream, &results);
  auto t1 = std::chrono::steady_clock::now();

  std::shared_mutex mtx;
  auto t2 = std::chrono::steady_clock::now();
  for (int i = 0; i < N; i++) {
    std::shared_lock<std::shared_mutex> lock(mtx);
    HFExecuteFaceTrack(session, stream, &results);
  }
  auto t3 = std::chrono::steady_clock::now();

  HFReleaseInspireFaceSession(session);

  double noLockUs = std::chrono::duration<double, std::micro>(t1 - t0).count() / N;
  double lockUs = std::chrono::duration<double, std::micro>(t3 - t2).count() / N;
  logf_("[fixed] avg HFExecuteFaceTrack latency: no-lock=%.1f us, shared-lock=%.1f us, delta=%.1f us (%.3f%%)",
        noLockUs, lockUs, lockUs - noLockUs, (lockUs - noLockUs) / noLockUs * 100.0);
}

static int cmdRepro(const char *modelPath, const char *imagePath, int cycles, bool guarded) {
  launchSDK(modelPath);
  HFImageStream stream = loadImageStream(imagePath);
  if (!stream) { logf_("[setup] could not create image stream"); return 2; }

  // Sanity check: one clean single-threaded track must find the face.
  {
    HFSession s = createSession();
    HFMultipleFaceData results{};
    HResult r = HFExecuteFaceTrack(s, stream, &results);
    logf_("[setup] sanity track: code=%ld detectedNum=%d", (long)r, results.detectedNum);
    if (r != HSUCCEED) return 2;
    HFReleaseInspireFaceSession(s);
  }

  if (guarded) measureLockOverhead(stream);

  std::mt19937 rng(std::random_device{}());
  for (int i = 1; i <= cycles; i++) {
    logf_("[%s] cycle %d/%d starting", guarded ? "fixed" : "repro", i, cycles);
    RaceResult res = guarded ? runGuardedCycle(stream, rng) : runUnguardedCycle(stream, rng);
    logf_("[%s] cycle %d/%d survived (tracks before release=%ld, calls after release=%ld, errors after=%ld)",
          guarded ? "fixed" : "repro", i, cycles,
          res.tracksBeforeRelease, res.tracksAfterRelease, res.errorsAfterRelease);
  }

  if (guarded) {
    logf_("[fixed] PASS: all %d guarded cycles completed cleanly", cycles);
    return 0;
  }
  logf_("[repro] NO CRASH after %d unguarded cycles (race did not fire this run)", cycles);
  return 1; // for repro, exiting alive means the race did NOT reproduce
}

// ---------------------------------------------------------------------------
// hub / hubfixed: FeatureHub insert/remove vs search
// ---------------------------------------------------------------------------

static std::vector<float> randomFeature(int len, std::mt19937 &rng) {
  std::normal_distribution<float> dist(0.f, 1.f);
  std::vector<float> f(len);
  double norm = 0;
  for (auto &v : f) { v = dist(rng); norm += (double)v * v; }
  norm = std::sqrt(norm);
  for (auto &v : f) v = (float)(v / norm);
  return f;
}

static int cmdHub(const char *modelPath, int seconds, bool guarded) {
  launchSDK(modelPath); // needed so HFGetFeatureLength/search params come from the pack

  HFFeatureHubConfiguration cfg{};
  cfg.primaryKeyMode = HF_PK_MANUAL_INPUT;
  cfg.enablePersistence = 0; // in-memory only
  cfg.persistenceDbPath = (HString) "";
  cfg.searchThreshold = 0.30f;
  cfg.searchMode = HF_SEARCH_MODE_EXHAUSTIVE; // scan the whole table -> widest race window
  HResult r = HFFeatureHubDataEnable(cfg);
  if (r != HSUCCEED) { logf_("[hub] HFFeatureHubDataEnable failed: %ld", (long)r); return 2; }

  HInt32 featLen = 0;
  r = HFGetFeatureLength(&featLen);
  if (r != HSUCCEED || featLen <= 0) { logf_("[hub] HFGetFeatureLength failed: %ld (len=%d)", (long)r, featLen); return 2; }
  logf_("[%s] feature length = %d, running %d seconds", guarded ? "hubfixed" : "hub", featLen, seconds);

  std::mutex hubMutex; // used only in guarded mode
  std::atomic<bool> stop{false};
  std::atomic<long> inserts{0}, removes{0}, searches{0}, hits{0}, insertErrs{0}, searchErrs{0};

  // Thread A: insert / remove random features with manual ids.
  std::thread writer([&]() {
    std::mt19937 rng(1234);
    int64_t nextId = 1;
    std::vector<int64_t> live;
    while (!stop.load(std::memory_order_acquire)) {
      std::vector<float> f = randomFeature(featLen, rng);
      HFFaceFeature feat{};
      feat.size = featLen;
      feat.data = f.data();
      HFFaceFeatureIdentity ident{};
      ident.id = nextId;
      ident.feature = &feat;
      HFaceId allocId = -1;
      HResult ir;
      if (guarded) {
        std::lock_guard<std::mutex> lock(hubMutex);
        ir = HFFeatureHubInsertFeature(ident, &allocId);
      } else {
        ir = HFFeatureHubInsertFeature(ident, &allocId);
      }
      if (ir == HSUCCEED) { live.push_back(nextId); inserts.fetch_add(1); }
      else insertErrs.fetch_add(1);
      nextId++;

      // Keep the table churning: remove a random live id once we have > 200.
      if (live.size() > 200) {
        std::uniform_int_distribution<size_t> pick(0, live.size() - 1);
        size_t idx = pick(rng);
        int64_t id = live[idx];
        HResult rr;
        if (guarded) {
          std::lock_guard<std::mutex> lock(hubMutex);
          rr = HFFeatureHubFaceRemove(id);
        } else {
          rr = HFFeatureHubFaceRemove(id);
        }
        if (rr == HSUCCEED) removes.fetch_add(1);
        live.erase(live.begin() + idx);
      }
    }
  });

  // Thread B: search with random features.
  std::thread searcher([&]() {
    std::mt19937 rng(5678);
    while (!stop.load(std::memory_order_acquire)) {
      std::vector<float> f = randomFeature(featLen, rng);
      HFFaceFeature query{};
      query.size = featLen;
      query.data = f.data();
      HFloat confidence = 0.f;
      HFFaceFeatureIdentity most{};
      HResult sr;
      if (guarded) {
        std::lock_guard<std::mutex> lock(hubMutex);
        sr = HFFeatureHubFaceSearch(query, &confidence, &most);
      } else {
        sr = HFFeatureHubFaceSearch(query, &confidence, &most);
      }
      if (sr == HSUCCEED) {
        searches.fetch_add(1);
        if (most.id != -1) hits.fetch_add(1);
      } else {
        searchErrs.fetch_add(1);
      }
    }
  });

  auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
  while (std::chrono::steady_clock::now() < deadline) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    HInt32 count = -1;
    if (guarded) {
      std::lock_guard<std::mutex> lock(hubMutex);
      HFFeatureHubGetFaceCount(&count);
    } else {
      HFFeatureHubGetFaceCount(&count);
    }
    logf_("[%s] alive: inserts=%ld removes=%ld searches=%ld hits=%ld insertErrs=%ld searchErrs=%ld tableCount=%d",
          guarded ? "hubfixed" : "hub", inserts.load(), removes.load(), searches.load(),
          hits.load(), insertErrs.load(), searchErrs.load(), count);
  }
  stop.store(true, std::memory_order_release);
  writer.join();
  searcher.join();

  logf_("[%s] COMPLETED %d seconds without crash: inserts=%ld removes=%ld searches=%ld hits=%ld insertErrs=%ld searchErrs=%ld",
        guarded ? "hubfixed" : "hub", seconds, inserts.load(), removes.load(), searches.load(),
        hits.load(), insertErrs.load(), searchErrs.load());
  HFFeatureHubDataDisable();
  return 0;
}

// ---------------------------------------------------------------------------

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr,
            "usage:\n"
            "  %s repro <modelPath> <imagePath> [cycles=50]\n"
            "  %s fixed <modelPath> <imagePath> [cycles=50]\n"
            "  %s hub <modelPath> [seconds=15]\n"
            "  %s hubfixed <modelPath> [seconds=15]\n",
            argv[0], argv[0], argv[0], argv[0]);
    return 2;
  }
  std::string cmd = argv[1];
  if (cmd == "repro" || cmd == "fixed") {
    if (argc < 4) { fprintf(stderr, "missing imagePath\n"); return 2; }
    int cycles = argc > 4 ? atoi(argv[4]) : 50;
    return cmdRepro(argv[2], argv[3], cycles, cmd == "fixed");
  }
  if (cmd == "hub" || cmd == "hubfixed") {
    int seconds = argc > 3 ? atoi(argv[3]) : 15;
    return cmdHub(argv[2], seconds, cmd == "hubfixed");
  }
  fprintf(stderr, "unknown subcommand: %s\n", cmd.c_str());
  return 2;
}
