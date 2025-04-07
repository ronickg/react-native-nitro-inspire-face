---
title: FaceRect
---

# FaceRect

Rectangle representing a face region in an image. Defines the position and size of a detected face.

```typescript
type FaceRect = {
  x: number;
  y: number;
  width: number;
  height: number;
};
```

## Properties

| Property | Type     | Description                                          |
| -------- | -------- | ---------------------------------------------------- |
| `x`      | `number` | X-coordinate of the top-left corner of the rectangle |
| `y`      | `number` | Y-coordinate of the top-left corner of the rectangle |
| `width`  | `number` | Width of the rectangle                               |
| `height` | `number` | Height of the rectangle                              |
