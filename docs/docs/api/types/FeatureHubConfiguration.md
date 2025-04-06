---
title: FeatureHubConfiguration
---

# FeatureHubConfiguration

Configuration settings for the FeatureHub face management system.

```typescript
type FeatureHubConfiguration = {
  searchMode: SearchMode;
  enablePersistence: boolean;
  persistenceDbPath: string;
  searchThreshold: number;
  primaryKeyMode: PrimaryKeyMode;
};
```

## Properties

### searchMode

- Determines how face searches are performed. See [SearchMode](../enums/SearchMode.md) for details.

### enablePersistence

- Controls whether face features are stored persistently in a database.

### persistenceDbPath

- File path where the face feature database will be stored when persistence is enabled.

### searchThreshold

- Similarity threshold for face matching (typically around 0.42 for access control scenarios).

### primaryKeyMode

- Controls how face feature IDs are generated. See [PrimaryKeyMode](../enums/PrimaryKeyMode.md) for details.
