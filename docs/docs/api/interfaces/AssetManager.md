---
sidebar_position: 5
title: AssetManager
---

# AssetManager

Interface for managing assets in the application. Provides functionality for accessing and manipulating files and directories across different platforms (iOS and Android).

```typescript
interface AssetManager
  extends HybridObject<{ ios: 'swift'; android: 'kotlin' }> {
  getFilesDirectory(): string;
  getDatabasesDirectory(): string;
  copyAssetToFile(assetPath: string, filePath: string): boolean;
}
```

## Methods

### getFilesDirectory

```typescript
getFilesDirectory(): string
```

Get the path to the application's files directory. This directory is suitable for storing application-specific files.

**Returns:**

- String path to the files directory

Defined in: `AssetManager.nitro.ts:13`

### getDatabasesDirectory

```typescript
getDatabasesDirectory(): string
```

Get the path to the application's databases directory. This directory is suitable for storing database files.

**Returns:**

- String path to the databases directory

Defined in: `AssetManager.nitro.ts:19`

### copyAssetToFile

```typescript
copyAssetToFile(assetPath: string, filePath: string): boolean
```

Copy an asset from the application bundle to a file in the filesystem. This is useful for extracting bundled resources to a location where they can be accessed and modified by the application.

**Parameters:**

- `assetPath`: Path to the asset in the application bundle
- `filePath`: Destination path in the filesystem

**Returns:**

- Boolean indicating success or failure of the copy operation

Defined in: `AssetManager.nitro.ts:27`
