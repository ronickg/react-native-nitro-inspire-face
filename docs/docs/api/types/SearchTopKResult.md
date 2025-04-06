---
title: SearchTopKResult
---

# SearchTopKResult

Result of a top-K face search operation.

```typescript
type SearchTopKResult = {
  confidence: number;
  id: number;
};
```

## Properties

### confidence

- Similarity confidence score between the search feature and matched face.

### id

- Unique identifier of the matched face in the database.
