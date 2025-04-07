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

### `writeImageToFile`

Decode the image from ImageStream and store it to a disk path.

```typescript
writeImageToFile(filePath: string): void
```

#### **Parameters**

| Name       | Type     | Description                            |
| ---------- | -------- | -------------------------------------- |
| `filePath` | `string` | The path to which the image is written |

#### **Returns**

- `void`

---

### `setFormat`

Set the format of the image stream.

```typescript
setFormat(format: ImageFormat): void
```

#### **Parameters**

| Name     | Type                                     | Description         |
| -------- | ---------------------------------------- | ------------------- |
| `format` | [`ImageFormat`](../enums/ImageFormat.md) | Format of the image |

#### **Returns**

- `void`

---

### `setRotation`

Set the rotation of the image stream.

```typescript
setRotation(rotation: CameraRotation): void
```

#### **Parameters**

| Name       | Type                                           | Description                 |
| ---------- | ---------------------------------------------- | --------------------------- |
| `rotation` | [`CameraRotation`](../enums/CameraRotation.md) | Rotation angle of the image |

#### **Returns**

- `void`

---

### `createImageBitmap`

Create a bitmap image from the stream.

```typescript
createImageBitmap(isRotate?: boolean, scale?: number): ImageBitmap
```

#### **Parameters**

| Name       | Type      | Description                              |
| ---------- | --------- | ---------------------------------------- |
| `isRotate` | `boolean` | _(Optional)_ Whether to rotate the image |
| `scale`    | `number`  | _(Optional)_ The scale of the image      |

#### **Returns**

- [`ImageBitmap`](./ImageBitmap.md) - The created bitmap image
