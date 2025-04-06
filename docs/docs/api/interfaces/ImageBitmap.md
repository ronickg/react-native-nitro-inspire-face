---
sidebar_position: 4
title: ImageBitmap
---

# ImageBitmap

Interface for handling bitmap image operations. Provides functionality for image manipulation and drawing.

```typescript
interface ImageBitmap {
  readonly width: number;
  readonly height: number;
  readonly channels: number;
  readonly data: ArrayBuffer;

  drawRect(rect: FaceRect, color: Color, thickness: number): void;
  drawCircleF(
    point: Point2f,
    radius: number,
    color: Color,
    thickness: number
  ): void;
  drawCircle(
    point: Point2i,
    radius: number,
    color: Color,
    thickness: number
  ): void;
}
```

## Properties

### width

- Width of the image in pixels.

### height

- Height of the image in pixels.

### channels

- Number of color channels in the image.

### data

- Raw image data as ArrayBuffer.

## Methods

### drawRect

- Draws a rectangle on the image.
- Parameters:
  - `rect`: [`FaceRect`](../types/FaceRect.md) - Rectangle coordinates and dimensions
  - `color`: [`Color`](../types/Color.md) - RGB color for drawing
  - `thickness`: `number` - Line thickness in pixels

### drawCircleF

- Draws a circle using floating-point coordinates.
- Parameters:
  - `point`: [`Point2f`](../types/Point2f.md) - Center point of the circle
  - `radius`: `number` - Radius of the circle
  - `color`: [`Color`](../types/Color.md) - RGB color for drawing
  - `thickness`: `number` - Line thickness in pixels

### drawCircle

- Draws a circle using integer coordinates.
- Parameters:
  - `point`: [`Point2i`](../types/Point2i.md) - Center point of the circle
  - `radius`: `number` - Radius of the circle
  - `color`: [`Color`](../types/Color.md) - RGB color for drawing
  - `thickness`: `number` - Line thickness in pixels
