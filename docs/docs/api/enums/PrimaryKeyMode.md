---
sidebar_position: 5
title: PrimaryKeyMode
---

# PrimaryKeyMode

Primary key generation modes for face feature management.

```typescript
enum PrimaryKeyMode {
  AUTO_INCREMENT = 0,
  MANUAL_INPUT = 1,
}
```

## Modes

### AUTO_INCREMENT

- Automatically generates incremental IDs for face features. Recommended for most use cases.

### MANUAL_INPUT

- Allows manual specification of IDs for face features. Useful when integrating with existing ID systems.
