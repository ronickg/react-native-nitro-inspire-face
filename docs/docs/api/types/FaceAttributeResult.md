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

- Age range prediction:
  - 0: 0-2 years old
  - 1: 3-9 years old
  - 2: 10-19 years old
  - 3: 20-29 years old
  - 4: 30-39 years old
  - 5: 40-49 years old
  - 6: 50-59 years old
  - 7: 60-69 years old
  - 8: 70+ years old

### gender

- Gender prediction:
  - 0: Female
  - 1: Male

### race

- Race/ethnicity prediction:
  - 0: Black
  - 1: Asian
  - 2: Latino/Hispanic
  - 3: Middle Eastern
  - 4: White
