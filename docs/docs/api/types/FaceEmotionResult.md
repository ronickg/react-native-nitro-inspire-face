---
title: FaceEmotionResult
---

# FaceEmotionResult

Results of face emotion recognition. Contains the predicted emotion for each detected face.

```typescript
type FaceEmotionResult = {
  emotion: number;
};
```

## Properties

| Property  | Type     | Description                                                                                          |
| --------- | -------- | ---------------------------------------------------------------------------------------------------- |
| `emotion` | `number` | Predicted emotion (0: Neutral, 1: Happy, 2: Sad, 3: Surprise, 4: Fear, 5: Disgust, 6: Anger) |
