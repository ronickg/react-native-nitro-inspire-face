---
title: FaceInteractionState
---

# FaceInteractionState

State information for face interaction detection. Used to track the state of eyes during interaction.

```typescript
type FaceInteractionState = {
  left: number;
  right: number;
};
```

## Properties

| Property | Type     | Description                                            |
| -------- | -------- | ------------------------------------------------------ |
| `left`   | `number` | State of the left eye (0-1, where 1 means fully open)  |
| `right`  | `number` | State of the right eye (0-1, where 1 means fully open) |
