---
sidebar_position: 3
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

## Values

| Enum             | Value | Description                                                                                            |
| ---------------- | ----- | ------------------------------------------------------------------------------------------------------ |
| `AUTO_INCREMENT` | `0`   | Automatically generates incremental IDs for face features. Recommended for most use cases              |
| `MANUAL_INPUT`   | `1`   | Allows manual specification of IDs for face features. Useful when integrating with existing ID systems |
