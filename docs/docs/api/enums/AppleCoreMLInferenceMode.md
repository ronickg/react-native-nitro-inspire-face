---
sidebar_position: 1
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

## Values

| Enum  | Value | Description                                                                                                                       |
| ----- | ----- | --------------------------------------------------------------------------------------------------------------------------------- |
| `CPU` | `0`   | Uses only CPU for inference computations. Most compatible but potentially slower option                                           |
| `GPU` | `1`   | Prioritizes GPU for inference computations. Better performance for parallel processing tasks                                      |
| `ANE` | `2`   | Automatic selection with Apple Neural Engine (ANE) prioritized. Best performance on supported devices with neural engine hardware |
