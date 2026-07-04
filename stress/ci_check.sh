#!/bin/bash
# Deterministic native regression gate. The GUARDED modes must complete without
# crashing — they exercise the exact use-during-dispose race (session) and the
# FeatureHub insert/search race against the committed InspireFace + MNN static
# slices, headless via `xcrun simctl spawn`. A crash here fails the build.
#
# The UNGUARDED diagnostics (repro/hub) are NOT run here — a no-crash run is a
# valid non-deterministic outcome, so they'd flake a PR. Run stress/run_all.sh
# for those (nightly / local signal).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/stress/stress_harness"
MODEL="$ROOT/example/ios/Megatron"   # git-tracked model pack
IMAGE="$ROOT/example/ios/kun.jpg"    # git-tracked test image
CYCLES="${STRESS_CYCLES:-50}"
HUB_SECONDS="${STRESS_HUB_SECONDS:-15}"
SIM="${SIM_UDID:-booted}"

[ -f "$MODEL" ] || { echo "missing model pack: $MODEL" >&2; exit 1; }
[ -f "$IMAGE" ] || { echo "missing test image: $IMAGE" >&2; exit 1; }
[ -f "$BIN" ] || bash "$ROOT/stress/build.sh"

run() { xcrun simctl spawn "$SIM" "$BIN" "$@"; }

echo "=== fixed: session use-during-dispose, $CYCLES cycles (must exit 0) ==="
run fixed "$MODEL" "$IMAGE" "$CYCLES"
echo "fixed PASSED"

echo "=== hubfixed: FeatureHub concurrency, ${HUB_SECONDS}s (must exit 0) ==="
run hubfixed "$MODEL" "$HUB_SECONDS"
echo "hubfixed PASSED"

echo "ALL STRESS REGRESSION CHECKS PASSED"
