---
title: FaceData
---

# FaceData

Comprehensive data structure for a detected face. Contains information about the face's position, tracking, confidence, orientation, and feature token.

```typescript
type FaceData = {
  rect: FaceRect;
  trackId: number;
  trackCount: number;
  detConfidence: number;
  angle: FaceEulerAngle;
  token: ArrayBuffer;
};
```

## Properties

| Property        | Type                                    | Description                                           |
| --------------- | --------------------------------------- | ----------------------------------------------------- |
| `rect`          | [`FaceRect`](./FaceRect.md)             | Rectangle defining the face region                    |
| `trackId`       | `number`                                | Unique identifier for tracking the face across frames |
| `trackCount`    | `number`                                | Number of frames this face has been tracked            |
| `detConfidence` | `number`                                | Confidence score of the face detection                |
| `angle`         | [`FaceEulerAngle`](./FaceEulerAngle.md) | 3D orientation of the face                            |
| `token`         | `ArrayBuffer`                           | Binary token containing face feature data             |
