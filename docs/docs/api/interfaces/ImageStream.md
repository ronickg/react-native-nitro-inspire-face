---
sidebar_position: 3
title: ImageStream
---

# ImageStream

Interface for handling image stream operations. Provides functionality for image stream manipulation and conversion.

```typescript
interface ImageStream {
  writeImageToFile(filePath: string): void;
  setFormat(format: ImageFormat): void;
  setRotation(rotation: CameraRotation): void;
  createImageBitmap(isRotate?: boolean, scale?: number): ImageBitmap;
}
```

## Methods

### writeImageToFile

- Writes the image stream to a file.
- Parameters:
  - `filePath`: `string` - Path where the image will be saved

### setFormat

- Sets the format of the image stream.
- Parameters:
  - `format`: [`ImageFormat`](../enums/ImageFormat.md) - Image format to use

### setRotation

- Sets the rotation of the image stream.
- Parameters:
  - `rotation`: [`CameraRotation`](../enums/CameraRotation.md) - Rotation angle to apply

### createImageBitmap

- Creates a bitmap image from the stream.
- Parameters:
  - `isRotate`: `boolean` (optional) - Whether to apply rotation
  - `scale`: `number` (optional) - Scale factor to apply
- Returns: [`ImageBitmap`](./ImageBitmap.md) - The created bitmap image
