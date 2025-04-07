---
sidebar_position: 4
title: SearchMode
---

# SearchMode

Search modes for face recognition operations.

```typescript
enum SearchMode {
  EAGER = 0,
  EXHAUSTIVE = 1,
}
```

## Values

| Enum         | Value | Description                                                                                                                               |
| ------------ | ----- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| `EAGER`      | `0`   | Stops searching when a face feature vector meets the threshold. Optimized for speed when finding the first acceptable match is sufficient |
| `EXHAUSTIVE` | `1`   | Searches through all face features to find the best possible match. More thorough but potentially slower than EAGER mode                  |
