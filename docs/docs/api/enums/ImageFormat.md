---
sidebar_position: 8
title: ImageFormat
---

# ImageFormat

Supported image formats for camera streams and image processing.

```typescript
enum ImageFormat {
  RGB = 0,
  BGR = 1,
  RGBA = 2,
  BGRA = 3,
  YUV_NV12 = 4,
  YUV_NV21 = 5,
}
```

## Values

| Enum       | Value | Description                                                                |
| ---------- | ----- | -------------------------------------------------------------------------- |
| `RGB`      | `0`   | Standard RGB color format with 3 channels                                  |
| `BGR`      | `1`   | BGR color format (OpenCV Mat default) with 3 channels                      |
| `RGBA`     | `2`   | RGB color format with alpha channel (4 channels)                           |
| `BGRA`     | `3`   | BGR color format with alpha channel (4 channels)                           |
| `YUV_NV12` | `4`   | YUV color format with NV12 layout, commonly used in video processing       |
| `YUV_NV21` | `5`   | YUV color format with NV21 layout, commonly used in Android camera outputs |
