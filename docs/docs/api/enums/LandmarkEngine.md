---
sidebar_position: 7
title: LandmarkEngine
---

# LandmarkEngine

Landmark engine options for face landmark detection. Different engines offer different accuracy/speed tradeoffs.

```typescript
enum LandmarkEngine {
  HYPLMV2_025 = 0,
  HYPLMV2_050 = 1,
  INSIGHTFACE_2D106 = 2,
}
```

## Values

| Enum              | Value | Description                                          |
| ----------------- | ----- | ---------------------------------------------------- |
| `HYPLMV2_025`     | `0`   | HypLMv2 0.25 - lightweight, fastest (default)       |
| `HYPLMV2_050`     | `1`   | HypLMv2 0.50 - balanced accuracy and speed          |
| `INSIGHTFACE_2D106` | `2` | InsightFace 2D106 track - highest accuracy, heavier |
