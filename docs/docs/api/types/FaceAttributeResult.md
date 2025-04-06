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

### ageBracket

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

### gender

| Value | Description |
| ----- | ----------- |
| 0     | Female      |
| 1     | Male        |

### race

| Value | Description     |
| ----- | --------------- |
| 0     | Black           |
| 1     | Asian           |
| 2     | Latino/Hispanic |
| 3     | Middle Eastern  |
| 4     | White           |
