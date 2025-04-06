---
title: FaceData
---

# FaceData

Comprehensive data structure for a detected face.

```typescript
type FaceData = {
  rect: FaceRect;
  trackId: number;
  detConfidence: number;
  angle: FaceEulerAngle;
  token: ArrayBuffer;
};
```

## Properties

### rect

- Bounding rectangle of the detected face. See [FaceRect](./FaceRect.md) for details.

### trackId

- Unique identifier for tracking this face across video frames.

### detConfidence

- Detection confidence score between 0 and 1.

### angle

- 3D orientation of the face. See [FaceEulerAngle](./FaceEulerAngle.md) for details.

### token

- Binary data containing face feature information for recognition.
