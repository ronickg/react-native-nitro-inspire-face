---
title: FeatureHubConfiguration
---

# FeatureHubConfiguration

Configuration settings for the FeatureHub functionality. FeatureHub is a built-in global lightweight face feature vector management system. It supports basic face feature search, deletion, and modification functions, and offers two optional data storage modes: in-memory and persistence.

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

| Property            | Type                                           | Description                                                    |
| ------------------- | ---------------------------------------------- | -------------------------------------------------------------- |
| `searchMode`        | [`SearchMode`](../enums/SearchMode.md)         | Mode of face search affecting execution efficiency and results |
| `enablePersistence` | `boolean`                                      | Flag to enable or disable data persistence                     |
| `persistenceDbPath` | `string`                                       | Path to the database file for persistence storage              |
| `searchThreshold`   | `number`                                       | Threshold value for face search comparisons. Default to 0.48   |
| `primaryKeyMode`    | [`PrimaryKeyMode`](../enums/PrimaryKeyMode.md) | Mode for managing primary keys in the database                 |
