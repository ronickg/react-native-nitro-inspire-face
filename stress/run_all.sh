#!/bin/bash
# Run the full stress matrix inside the booted iOS simulator.
# repro is expected to CRASH (that's a successful reproduction);
# fixed / hub / hubfixed are expected to exit 0.
set -uo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/stress/stress_harness"
MODEL="$ROOT/example/ios/Megatron"
IMAGE="$ROOT/example/ios/kun.jpg"
SIM="${SIM_UDID:-booted}"
REPRO_RUNS="${REPRO_RUNS:-5}"

spawn() { xcrun simctl spawn "$SIM" "$BIN" "$@"; }

echo "=== repro (unguarded race) — $REPRO_RUNS runs, up to 50 cycles each ==="
crashes=0
for i in $(seq 1 "$REPRO_RUNS"); do
  spawn repro "$MODEL" "$IMAGE" 50 > "/tmp/stress_repro_$i.log" 2>&1
  code=$?
  if [ "$code" -ge 128 ]; then
    sig=$((code - 128))
    crashes=$((crashes + 1))
    cycle=$(grep -c "cycle .* starting" "/tmp/stress_repro_$i.log")
    echo "run $i: CRASHED with signal $sig (exit $code) on cycle $cycle  <-- race reproduced"
  else
    echo "run $i: exited $code without crashing (race did not fire)"
  fi
done
echo "repro summary: $crashes/$REPRO_RUNS runs crashed"
echo

echo "=== fixed (shared_mutex guard) — 50 cycles, must exit 0 ==="
spawn fixed "$MODEL" "$IMAGE" 50 2>&1 | grep -E "\[fixed\]|sanity"
code=${PIPESTATUS[0]}
echo "fixed exit code: $code"
echo

echo "=== hub (unguarded FeatureHub insert/remove vs search, 15s) ==="
spawn hub "$MODEL" 15 2>&1 | grep -E "\[hub\]"
code=${PIPESTATUS[0]}
echo "hub exit code: $code $([ "$code" -ge 128 ] && echo "(signal $((code-128)))")"
echo

echo "=== hubfixed (mutex-guarded FeatureHub, 15s, must exit 0) ==="
spawn hubfixed "$MODEL" 15 2>&1 | grep -E "\[hubfixed\]"
code=${PIPESTATUS[0]}
echo "hubfixed exit code: $code"
