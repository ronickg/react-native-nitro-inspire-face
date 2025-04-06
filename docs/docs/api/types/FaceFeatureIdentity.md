---
title: FaceFeatureIdentity
---

# FaceFeatureIdentity

Identity information associated with a face feature.

```typescript
type FaceFeatureIdentity = {
  id: number;
  feature: ArrayBuffer;
  confidence?: number;
};
```

## Properties

### id

- Unique identifier for the face feature in the database.

### feature

- Binary feature vector representing the face's unique characteristics.

### confidence

- Optional confidence score for the feature match when returned from search operations.
