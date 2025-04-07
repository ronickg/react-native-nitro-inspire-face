---
title: FaceAttributeResult
---

# FaceAttributeResult

Results of face attribute analysis including demographic predictions.

```typescript
type FaceAttributeResult = {
  ageBracket: number;
  gender: number;
  race: number;
};
```

## Properties

| Property     | Type     | Description                                      |
| ------------ | -------- | ------------------------------------------------ |
| `ageBracket` | `number` | Age range prediction (0-8, see table below)      |
| `gender`     | `number` | Gender prediction (0-1, see table below)         |
| `race`       | `number` | Race/ethnicity prediction (0-4, see table below) |

### Age Bracket Values

| Value | Age Range   |
| ----- | ----------- |
| 0     | 0-2 years   |
| 1     | 3-9 years   |
| 2     | 10-19 years |
| 3     | 20-29 years |
| 4     | 30-39 years |
| 5     | 40-49 years |
| 6     | 50-59 years |
| 7     | 60-69 years |
| 8     | 70+ years   |

### Gender Values

| Value | Description |
| ----- | ----------- |
| 0     | Female      |
| 1     | Male        |

### Race Values

| Value | Description     |
| ----- | --------------- |
| 0     | Black           |
| 1     | Asian           |
| 2     | Latino/Hispanic |
| 3     | Middle Eastern  |
| 4     | White           |
