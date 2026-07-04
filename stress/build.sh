#!/bin/bash
# Build the InspireFace stress harness for the iOS simulator (arm64).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
STRESS="$ROOT/stress"
IF_SLICE="$ROOT/ios/InspireFace.xcframework/ios-arm64_x86_64-simulator"
MNN_SLICE="$ROOT/ios/MNN.xcframework/ios-arm64_x86_64-simulator"

xcrun -sdk iphonesimulator clang++ \
  -target arm64-apple-ios17.0-simulator \
  -std=c++17 -O1 -g \
  -fobjc-arc \
  "$STRESS/main.mm" \
  -I "$IF_SLICE/Headers" \
  -F "$MNN_SLICE" \
  "$IF_SLICE/libInspireFace.a" \
  -framework MNN \
  -framework Foundation \
  -framework CoreML \
  -framework Accelerate \
  -framework Metal \
  -framework CoreVideo \
  -framework CoreGraphics \
  -framework ImageIO \
  -framework CoreFoundation \
  -lc++ -lsqlite3 -lz \
  -o "$STRESS/stress_harness"

echo "Built: $STRESS/stress_harness"
