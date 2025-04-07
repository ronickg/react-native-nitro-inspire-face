---
sidebar_position: 4
title: ImageBitmap
---

# ImageBitmap

Interface for handling bitmap image operations. Provides functionality for image manipulation and drawing.

## Properties

### `width`

Width of the image in pixels.

```typescript
readonly width: number
```

### `height`

Height of the image in pixels.

```typescript
readonly height: number
```

### `channels`

Number of color channels in the image.

```typescript
readonly channels: number
```

### `data`

Raw image data as ArrayBuffer.

```typescript
readonly data: ArrayBuffer
```

## Methods

### `drawRect`

Draw a rectangle on the image.

```typescript
drawRect(rect: FaceRect, color: Color, thickness: number): void
```

#### **Parameters**

| Name        | Type                               | Description                          |
| ----------- | ---------------------------------- | ------------------------------------ |
| `rect`      | [`FaceRect`](../types/FaceRect.md) | Rectangle coordinates and dimensions |
| `color`     | [`Color`](../types/Color.md)       | RGB color for drawing                |
| `thickness` | `number`                           | Line thickness in pixels             |

#### **Returns**

- `void`

---

### `drawCircleF`

Draw a circle using floating-point coordinates.

```typescript
drawCircleF(point: Point2f, radius: number, color: Color, thickness: number): void
```

#### **Parameters**

| Name        | Type                             | Description                |
| ----------- | -------------------------------- | -------------------------- |
| `point`     | [`Point2f`](../types/Point2f.md) | Center point of the circle |
| `radius`    | `number`                         | Radius of the circle       |
| `color`     | [`Color`](../types/Color.md)     | RGB color for drawing      |
| `thickness` | `number`                         | Line thickness in pixels   |

#### **Returns**

- `void`

---

### `drawCircle`

Draw a circle using integer coordinates.

```typescript
drawCircle(point: Point2i, radius: number, color: Color, thickness: number): void
```

#### **Parameters**

| Name        | Type                             | Description                |
| ----------- | -------------------------------- | -------------------------- |
| `point`     | [`Point2i`](../types/Point2i.md) | Center point of the circle |
| `radius`    | `number`                         | Radius of the circle       |
| `color`     | [`Color`](../types/Color.md)     | RGB color for drawing      |
| `thickness` | `number`                         | Line thickness in pixels   |

#### **Returns**

- `void`
