---
title: SearchTopKResult
---

# SearchTopKResult

Result of a top-K search operation in face recognition. Contains the confidence score and ID of a matched face.

```typescript
type SearchTopKResult = {
  confidence: number;
  id: number;
};
```

## Properties

| Property     | Type     | Description                   |
| ------------ | -------- | ----------------------------- |
| `confidence` | `number` | Confidence score of the match |
| `id`         | `number` | ID of the matched face        |
