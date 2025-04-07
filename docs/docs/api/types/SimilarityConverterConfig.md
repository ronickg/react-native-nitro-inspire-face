---
title: SimilarityConverterConfig
---

# SimilarityConverterConfig

Configuration for converting cosine similarity scores to percentage-based similarity.

```typescript
type SimilarityConverterConfig = {
  threshold: number;
  middleScore: number;
  steepness: number;
  outputMin: number;
  outputMax: number;
};
```

## Properties

| Property      | Type     | Description                                                                      |
| ------------- | -------- | -------------------------------------------------------------------------------- |
| `threshold`   | `number` | Cosine similarity threshold for determining face matches (typically around 0.42) |
| `middleScore` | `number` | Reference value for percentage conversion (typically 0.5 or 0.6)                 |
| `steepness`   | `number` | Controls the steepness of the conversion curve (typically around 8.0)            |
| `outputMin`   | `number` | Minimum value of the output range (typically 0.01)                               |
| `outputMax`   | `number` | Maximum value of the output range (typically 1.0)                                |
