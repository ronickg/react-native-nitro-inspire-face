---
title: FaceInteractionsAction
---

# FaceInteractionsAction

Detection results for various face interaction actions. Used to track different types of facial movements and gestures.

```typescript
type FaceInteractionsAction = {
  normal: number;
  shake: number;
  jawOpen: number;
  headRaise: number;
  blink: number;
};
```

## Properties

| Property    | Type     | Description                             |
| ----------- | -------- | --------------------------------------- |
| `normal`    | `number` | Detection score for normal face state   |
| `shake`     | `number` | Detection score for head shake action   |
| `jawOpen`   | `number` | Detection score for jaw opening action  |
| `headRaise` | `number` | Detection score for head raising action |
| `blink`     | `number` | Detection score for eye blinking action |
