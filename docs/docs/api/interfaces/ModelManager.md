---
sidebar_position: 6
title: ModelManager
---

# ModelManager

Interface for managing remote face recognition model downloads and caching. Provides functionality for downloading models from CDNs like Cloudflare R2, verifying checksums, and managing local cache.

## Methods

### `downloadModel`

Download a model from a remote URL with progress callbacks. Downloads to a temporary location, verifies the checksum, then moves to cache.

```typescript
downloadModel(
  url: string,
  checksum: string,
): Promise<string>
```

#### **Parameters**

| Name         | Type                                                       | Description                                                              |
| ------------ | ---------------------------------------------------------- | ------------------------------------------------------------------------ |
| `url`        | `string`                                                   | Remote URL to download model from                                        |
| `checksum`   | `string`                                                   | Expected SHA-256 checksum (lowercase hex string, 64 chars)               |

#### **Returns**

- `Promise<string>` - Promise resolving to local file path of cached model

#### **Throws**

- `Error` - If download fails, checksum mismatch, or disk full

#### **Example**

```typescript
const path = await ModelManager.downloadModel(
  'https://r2.cloudflare.com/models/Megatron',
  'a3f7c8d2e1b4f9a6...',
  (bytes, total) => console.log(`${bytes}/${total}`)
);
```

---

### `getCachedModel`

Check if a model exists in cache and verify its checksum. If the cached file's checksum doesn't match, it's deleted and null is returned.

```typescript
getCachedModel(checksum: string): string | null
```

#### **Parameters**

| Name       | Type     | Description              |
| ---------- | -------- | ------------------------ |
| `checksum` | `string` | SHA-256 checksum to look up |

#### **Returns**

- `string | null` - Local file path if cached and valid, null otherwise

#### **Example**

```typescript
const cachedPath = ModelManager.getCachedModel('a3f7c8d2...');
if (cachedPath) {
  // Model is cached and valid
  InspireFace.launch(cachedPath);
}
```
