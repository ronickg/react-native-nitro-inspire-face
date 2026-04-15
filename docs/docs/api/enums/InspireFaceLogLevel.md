---
sidebar_position: 8
title: InspireFaceLogLevel
---

# InspireFaceLogLevel

SDK built-in log level mode for controlling log verbosity.

```typescript
enum InspireFaceLogLevel {
  LOG_NONE = 0,
  LOG_DEBUG = 1,
  LOG_INFO = 2,
  LOG_WARN = 3,
  LOG_ERROR = 4,
  LOG_FATAL = 5,
}
```

## Values

| Enum        | Value | Description                                                                         |
| ----------- | ----- | ----------------------------------------------------------------------------------- |
| `LOG_NONE`  | `0`   | No logging, disables all log output                                                 |
| `LOG_DEBUG` | `1`   | Debug level for detailed system information mostly useful for developers             |
| `LOG_INFO`  | `2`   | Information level for general system information about operational status            |
| `LOG_WARN`  | `3`   | Warning level for non-critical issues that might need attention                      |
| `LOG_ERROR` | `4`   | Error level for error events that might still allow the application to continue      |
| `LOG_FATAL` | `5`   | Fatal level for severe error events that will presumably lead the application to abort |
