---
title: FaceInteractionsAction
---

# FaceInteractionsAction

Detection results for various face interaction actions.

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

### normal

- Detection score for normal face state (neutral expression).

### shake

- Detection score for head shake action (left-right movement).

### jawOpen

- Detection score for jaw opening action (mouth opening).

### headRaise

- Detection score for head raising action (looking up).

### blink

- Detection score for eye blinking action.
