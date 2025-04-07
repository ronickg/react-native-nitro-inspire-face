---
title: FaceEulerAngle
---

# FaceEulerAngle

Euler angles representing the orientation of a face in 3D space. These angles describe the rotation of the face along different axes.

```typescript
type FaceEulerAngle = {
  roll: number;
  yaw: number;
  pitch: number;
};
```

## Properties

| Property | Type     | Description                                          |
| -------- | -------- | ---------------------------------------------------- |
| `roll`   | `number` | Roll angle of the face (rotation around the Z-axis)  |
| `yaw`    | `number` | Yaw angle of the face (rotation around the Y-axis)   |
| `pitch`  | `number` | Pitch angle of the face (rotation around the X-axis) |
