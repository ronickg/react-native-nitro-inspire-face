---
sidebar_position: 6
title: AppleCoreMLInferenceMode
---

# AppleCoreMLInferenceMode

Inference modes for Apple CoreML on supported devices.

```typescript
enum AppleCoreMLInferenceMode {
  CPU = 0,
  GPU = 1,
  ANE = 2,
}
```

## Modes

### CPU

Uses only CPU for inference computations. Most compatible but potentially slower option.

### GPU

Prioritizes GPU for inference computations. Better performance for parallel processing tasks.

### ANE

Automatic selection with Apple Neural Engine (ANE) prioritized. Best performance on supported devices with neural engine hardware.
