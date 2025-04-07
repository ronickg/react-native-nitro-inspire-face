---
sidebar_position: 5
title: AssetManager
---

# AssetManager

Interface for managing assets in the application. Provides functionality for accessing and manipulating files and directories across different platforms (iOS and Android).

## Methods

### `getFilesDirectory`

Get the path to the application's files directory.

```typescript
getFilesDirectory(): string
```

#### **Returns**

- `string` - String path to the files directory

---

### `getDatabasesDirectory`

Get the path to the application's databases directory.

```typescript
getDatabasesDirectory(): string
```

#### **Returns**

- `string` - String path to the databases directory

---

### `copyAssetToFile`

Copy an asset from the application bundle to a file in the filesystem.

```typescript
copyAssetToFile(assetPath: string, filePath: string): boolean
```

#### **Parameters**

| Name        | Type     | Description                                 |
| ----------- | -------- | ------------------------------------------- |
| `assetPath` | `string` | Path to the asset in the application bundle |
| `filePath`  | `string` | Destination path in the filesystem          |

#### **Returns**

- `boolean` - Boolean indicating success or failure of the copy operation
