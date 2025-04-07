---
sidebar_position: 2
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

## Values

| Enum                 | Value | Description                                                                                      |
| -------------------- | ----- | ------------------------------------------------------------------------------------------------ |
| `ALWAYS_DETECT`      | `0`   | Image detection mode that always detects faces, suitable for static images                       |
| `LIGHT_TRACK`        | `1`   | Video detection mode with face tracking, ideal for video streaming and front camera applications |
| `TRACK_BY_DETECTION` | `2`   | Video detection mode with face tracking, designed for high-resolution monitoring and capturing   |
