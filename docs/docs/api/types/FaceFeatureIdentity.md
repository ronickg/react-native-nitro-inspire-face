---
title: FaceFeatureIdentity
---

# FaceFeatureIdentity

Identity information associated with a face feature. Used for face recognition and database management.

```typescript
type FaceFeatureIdentity = {
  id: number;
  feature: ArrayBuffer;
  confidence?: number;
};
```

## Properties

| Property     | Type          | Description                                   |
| ------------ | ------------- | --------------------------------------------- |
| `id`         | `number`      | Unique identifier for the face feature        |
| `feature`    | `ArrayBuffer` | Feature vector representing the face          |
| `confidence` | `number`      | _(Optional)_ Confidence score for the feature |
