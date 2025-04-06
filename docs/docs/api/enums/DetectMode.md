---
sidebar_position: 3
title: DetectMode
---

# DetectMode

Face detection modes for different use cases.

```typescript
enum DetectMode {
  ALWAYS_DETECT = 0,
  LIGHT_TRACK = 1,
  TRACK_BY_DETECTION = 2,
}
```

## Modes

### ALWAYS_DETECT

- **Description**: Image detection mode that always detects faces, suitable for static images.

### LIGHT_TRACK

- **Description**: Video detection mode with face tracking, ideal for video streaming and front camera applications.

### TRACK_BY_DETECTION

- **Description**: Video detection mode with face tracking, designed for high-resolution monitoring and capturing. Requires specific compile-time options to be enabled.
