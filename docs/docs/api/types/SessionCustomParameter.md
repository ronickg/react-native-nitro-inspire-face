---
title: SessionCustomParameter
---

# SessionCustomParameter

Configuration options for face recognition session features.

```typescript
type SessionCustomParameter = {
  enableRecognition?: boolean;
  enableLiveness?: boolean;
  enableIrLiveness?: boolean;
  enableMaskDetect?: boolean;
  enableFaceQuality?: boolean;
  enableFaceAttribute?: boolean;
  enableInteractionLiveness?: boolean;
  enableDetectModeLandmark?: boolean;
};
```

## Properties

### enableRecognition

- Enables face recognition feature for comparing and matching faces.

### enableLiveness

- Enables RGB liveness detection to prevent spoofing attacks using photos or videos.

### enableIrLiveness

- Enables infrared liveness detection for enhanced security in supported devices.

### enableMaskDetect

- Enables detection of face masks or coverings.

### enableFaceQuality

- Enables assessment of face image quality for better recognition accuracy.

### enableFaceAttribute

- Enables prediction of facial attributes like age, gender, and race.

### enableInteractionLiveness

- Enables interactive liveness detection requiring user gestures or movements.

### enableDetectModeLandmark

- Enables facial landmark detection in detection mode.
