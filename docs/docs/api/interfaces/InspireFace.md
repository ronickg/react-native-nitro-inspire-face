---
sidebar_position: 1
title: InspireFace
---

# InspireFace

Main interface for the InspireFace SDK functionality. Provides comprehensive face detection, recognition, and analysis capabilities.

## Properties

### `version`

Version string of the SDK.

```typescript
readonly version: string
```

### `featureLength`

Length of face feature vectors used in face recognition.

```typescript
readonly featureLength: number
```

### `faceDenseLandmarkLength`

Number of dense facial landmarks that can be detected.

```typescript
readonly faceDenseLandmarkLength: number
```

### `faceBasicTokenLength`

Length of basic face tokens used in face detection and analysis.

```typescript
readonly faceBasicTokenLength: number
```

## Methods

### `launch`

Start the InspireFace SDK at the initialization stage of your program. This is a global operation designed to be used only once. It serves as a prerequisite for other function interfaces.

```typescript
launch(path: string): void
```

#### **Parameters**

| Name   | Type     | Description                                       |
| ------ | -------- | ------------------------------------------------- |
| `path` | `string` | Path to the resource files that need to be loaded |

#### **Returns**

- `void`

---

### `reload`

Reload the InspireFace SDK, releasing all allocated resources and reinitializing with new resources.

```typescript
reload(path: string): void
```

#### **Parameters**

| Name   | Type     | Description                                       |
| ------ | -------- | ------------------------------------------------- |
| `path` | `string` | Path to the resource files that need to be loaded |

#### **Returns**

- `void`

---

### `terminate`

Terminate the InspireFace SDK and release all allocated resources. This should be called at the end of your program to ensure proper cleanup.

```typescript
terminate(): void
```

#### **Returns**

- `void`

---

### `featureHubDataEnable`

Enable FeatureHub data management with specified configuration.

```typescript
featureHubDataEnable(config: FeatureHubConfiguration): void
```

#### **Parameters**

| Name     | Type                                                             | Description                           |
| -------- | ---------------------------------------------------------------- | ------------------------------------- |
| `config` | [`FeatureHubConfiguration`](../types/FeatureHubConfiguration.md) | Configuration settings for FeatureHub |

#### **Returns**

- `void`

---

### `featureHubDataDisable`

Disable FeatureHub data management.

```typescript
featureHubDataDisable(): void
```

#### **Returns**

- `void`

---

### `featureHubFaceSearchThresholdSetting`

Set the threshold for face search operations in FeatureHub.

```typescript
featureHubFaceSearchThresholdSetting(threshold: number): void
```

#### **Parameters**

| Name        | Type     | Description                                |
| ----------- | -------- | ------------------------------------------ |
| `threshold` | `number` | Threshold value for face search operations |

#### **Returns**

- `void`

---

### `createSession`

Create a new face recognition session with specified parameters.

```typescript
createSession(
  parameter: SessionCustomParameter,
  detectMode: DetectMode,
  maxDetectFaceNum: number,
  detectPixelLevel: number,
  trackByDetectModeFPS: number
): Session
```

#### **Parameters**

| Name                   | Type                                                           | Description                                                                             |
| ---------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| `parameter`            | [`SessionCustomParameter`](../types/SessionCustomParameter.md) | Custom parameters for the session                                                       |
| `detectMode`           | [`DetectMode`](../enums/DetectMode.md)                         | Face detection mode (ALWAYS_DETECT, LIGHT_TRACK, or TRACK_BY_DETECTION)                 |
| `maxDetectFaceNum`     | `number`                                                       | Maximum number of faces to detect                                                       |
| `detectPixelLevel`     | `number`                                                       | Detection resolution level (multiple of 160, e.g., 160, 320, 640; default -1 means 320) |
| `trackByDetectModeFPS` | `number`                                                       | Frame rate for tracking mode (default -1 means 30fps)                                   |

#### **Returns**

- [`Session`](./Session.md) - New session instance

---

### `createImageBitmapFromBuffer`

Create an image bitmap from a raw buffer.

```typescript
createImageBitmapFromBuffer(
  buffer: ArrayBuffer,
  width: number,
  height: number,
  channels: number
): ImageBitmap
```

#### **Parameters**

| Name       | Type          | Description              |
| ---------- | ------------- | ------------------------ |
| `buffer`   | `ArrayBuffer` | Raw image data           |
| `width`    | `number`      | Image width in pixels    |
| `height`   | `number`      | Image height in pixels   |
| `channels` | `number`      | Number of color channels |

#### **Returns**

- [`ImageBitmap`](./ImageBitmap.md) - Created bitmap image

---

### `createImageBitmapFromFilePath`

Create an image bitmap from a file path.

```typescript
createImageBitmapFromFilePath(
  channels: number,
  filePath: string
): ImageBitmap
```

#### **Parameters**

| Name       | Type     | Description              |
| ---------- | -------- | ------------------------ |
| `channels` | `number` | Number of color channels |
| `filePath` | `string` | Path to the image file   |

#### **Returns**

- [`ImageBitmap`](./ImageBitmap.md) - Created bitmap image

---

### `createImageStreamFromBitmap`

Create an image stream from a bitmap with specified rotation.

```typescript
createImageStreamFromBitmap(
  bitmap: ImageBitmap,
  rotation: CameraRotation
): ImageStream
```

#### **Parameters**

| Name       | Type                                           | Description         |
| ---------- | ---------------------------------------------- | ------------------- |
| `bitmap`   | [`ImageBitmap`](./ImageBitmap.md)              | Source bitmap image |
| `rotation` | [`CameraRotation`](../enums/CameraRotation.md) | Rotation to apply   |

#### **Returns**

- [`ImageStream`](./ImageStream.md) - Created image stream

---

### `getFaceDenseLandmarkFromFaceToken`

Get dense facial landmarks from a face token.

```typescript
getFaceDenseLandmarkFromFaceToken(
  token: ArrayBuffer,
  num?: number
): Point2f[]
```

#### **Parameters**

| Name    | Type          | Description                                  |
| ------- | ------------- | -------------------------------------------- |
| `token` | `ArrayBuffer` | Face token data                              |
| `num`   | `number`      | _(Optional)_ number of landmarks to retrieve |

#### **Returns**

- [`Point2f[]`](../types/Point2f.md) - Array of facial landmarks

---

### `getFaceFiveKeyPointsFromFaceToken`

Get five key facial points from a face token.

```typescript
getFaceFiveKeyPointsFromFaceToken(
  token: ArrayBuffer,
  num?: number
): Point2f[]
```

#### **Parameters**

| Name    | Type          | Description                               |
| ------- | ------------- | ----------------------------------------- |
| `token` | `ArrayBuffer` | Face token data                           |
| `num`   | `number`      | _(Optional)_ number of points to retrieve |

#### **Returns**

- [`Point2f[]`](../types/Point2f.md) - Array of facial points

---

### `featureHubFaceInsert`

Insert a face feature into the database.

```typescript
featureHubFaceInsert(feature: FaceFeatureIdentity): number
```

#### **Parameters**

| Name      | Type                                                     | Description                     |
| --------- | -------------------------------------------------------- | ------------------------------- |
| `feature` | [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | Face feature identity to insert |

#### **Returns**

- `number` - ID of the inserted feature

---

### `featureHubFaceUpdate`

Update a face feature in the database.

```typescript
featureHubFaceUpdate(feature: FaceFeatureIdentity): boolean
```

#### **Parameters**

| Name      | Type                                                     | Description                     |
| --------- | -------------------------------------------------------- | ------------------------------- |
| `feature` | [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | Face feature identity to update |

#### **Returns**

- `boolean` - Success status

---

### `featureHubFaceRemove`

Remove a face feature from the database.

```typescript
featureHubFaceRemove(id: number): boolean
```

#### **Parameters**

| Name | Type     | Description                      |
| ---- | -------- | -------------------------------- |
| `id` | `number` | ID of the face feature to remove |

#### **Returns**

- `boolean` - Success status

---

### `featureHubFaceSearch`

Search for a matching face feature in the database.

```typescript
featureHubFaceSearch(feature: ArrayBuffer): FaceFeatureIdentity | null
```

#### **Parameters**

| Name      | Type          | Description                  |
| --------- | ------------- | ---------------------------- |
| `feature` | `ArrayBuffer` | Feature vector to search for |

#### **Returns**

- [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | `null` - Matched feature or null if not found

---

### `featureHubGetFaceIdentity`

Get a face feature by ID from the database.

```typescript
featureHubGetFaceIdentity(id: number): FaceFeatureIdentity | null
```

#### **Parameters**

| Name | Type     | Description                        |
| ---- | -------- | ---------------------------------- |
| `id` | `number` | ID of the face feature to retrieve |

#### **Returns**

- [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | `null` - Retrieved feature or null if not found

---

### `featureHubFaceSearchTopK`

Search for top K similar face features.

```typescript
featureHubFaceSearchTopK(
  feature: ArrayBuffer,
  topK: number
): SearchTopKResult[]
```

#### **Parameters**

| Name      | Type          | Description                  |
| --------- | ------------- | ---------------------------- |
| `feature` | `ArrayBuffer` | Feature vector to search for |
| `topK`    | `number`      | Number of results to return  |

#### **Returns**

- [`SearchTopKResult[]`](../types/SearchTopKResult.md) - Array of search results

---

### `featureHubGetFaceCount`

Get the total count of face features in the database.

```typescript
featureHubGetFaceCount(): number
```

#### **Returns**

- `number` - Number of features in the database

---

### `featureHubGetExistingIds`

Get all existing face feature IDs from the database.

```typescript
featureHubGetExistingIds(): number[]
```

#### **Returns**

- `number[]` - Array of feature IDs

---

### `faceComparison`

Compare two face features.

```typescript
faceComparison(feature1: ArrayBuffer, feature2: ArrayBuffer): number
```

#### **Parameters**

| Name       | Type          | Description           |
| ---------- | ------------- | --------------------- |
| `feature1` | `ArrayBuffer` | First feature vector  |
| `feature2` | `ArrayBuffer` | Second feature vector |

#### **Returns**

- `number` - Similarity score between the two features

---

### `getRecommendedCosineThreshold`

Get the recommended threshold for cosine similarity comparison.

```typescript
getRecommendedCosineThreshold(): number
```

#### **Returns**

- `number` - Recommended threshold value

---

### `cosineSimilarityConvertToPercentage`

Convert cosine similarity to percentage similarity.

```typescript
cosineSimilarityConvertToPercentage(similarity: number): number
```

#### **Parameters**

| Name         | Type     | Description             |
| ------------ | -------- | ----------------------- |
| `similarity` | `number` | Cosine similarity value |

#### **Returns**

- `number` - Percentage similarity value

---

### `updateCosineSimilarityConverter`

Update the similarity converter configuration.

```typescript
updateCosineSimilarityConverter(config: SimilarityConverterConfig): void
```

#### **Parameters**

| Name     | Type                                                                 | Description                |
| -------- | -------------------------------------------------------------------- | -------------------------- |
| `config` | [`SimilarityConverterConfig`](../types/SimilarityConverterConfig.md) | New configuration settings |

#### **Returns**

- `void`

---

### `getCosineSimilarityConverter`

Get the current similarity converter configuration.

```typescript
getCosineSimilarityConverter(): SimilarityConverterConfig
```

#### **Returns**

- [`SimilarityConverterConfig`](../types/SimilarityConverterConfig.md) - Current configuration

---

### `setExpansiveHardwareRockchipDmaHeapPath`

Set the path for Rockchip DMA heap. By default, the DMA Heap address used by RGA on RK devices is pre-configured.

```typescript
setExpansiveHardwareRockchipDmaHeapPath(path: string): void
```

#### **Parameters**

| Name   | Type     | Description      |
| ------ | -------- | ---------------- |
| `path` | `string` | Path to DMA heap |

#### **Returns**

- `void`

---

### `queryExpansiveHardwareRockchipDmaHeapPath`

Query the current Rockchip DMA heap path.

```typescript
queryExpansiveHardwareRockchipDmaHeapPath(): string
```

#### **Returns**

- `string` - Current DMA heap path

---

### `setAppleCoreMLInferenceMode`

Set the Apple CoreML inference mode. Must be called before creating a session.

```typescript
setAppleCoreMLInferenceMode(mode: AppleCoreMLInferenceMode): void
```

#### **Parameters**

| Name   | Type                                                               | Description           |
| ------ | ------------------------------------------------------------------ | --------------------- |
| `mode` | [`AppleCoreMLInferenceMode`](../enums/AppleCoreMLInferenceMode.md) | Inference mode to use |

#### **Returns**

- `void`

---

### `setCudaDeviceId`

Set the CUDA device ID. Must be called before creating a session.

```typescript
setCudaDeviceId(deviceId: number): void
```

#### **Parameters**

| Name       | Type     | Description           |
| ---------- | -------- | --------------------- |
| `deviceId` | `number` | ID of the CUDA device |

#### **Returns**

- `void`

---

### `getCudaDeviceId`

Get the current CUDA device ID.

```typescript
getCudaDeviceId(): number
```

#### **Returns**

- `number` - Current CUDA device ID

---

### `printCudaDeviceInfo`

Print information about the CUDA device.

```typescript
printCudaDeviceInfo(): void
```

#### **Returns**

- `void`

---

### `getNumCudaDevices`

Get the number of available CUDA devices.

```typescript
getNumCudaDevices(): number
```

#### **Returns**

- `number` - Number of available CUDA devices

---

### `checkCudaDeviceSupport`

Check if CUDA device support is available.

```typescript
checkCudaDeviceSupport(): boolean
```

#### **Returns**

- `boolean` - Whether CUDA device support is available

---

### `fromBase64`

Convert a base64 string to an ArrayBuffer.

```typescript
fromBase64(base64: string): ArrayBuffer
```

#### **Parameters**

| Name     | Type     | Description              |
| -------- | -------- | ------------------------ |
| `base64` | `string` | Base64 string to convert |

#### **Returns**

- `ArrayBuffer` - Converted data

---

### `toBase64`

Convert an ArrayBuffer to a base64 string.

```typescript
toBase64(buffer: ArrayBuffer): string
```

#### **Parameters**

| Name     | Type          | Description            |
| -------- | ------------- | ---------------------- |
| `buffer` | `ArrayBuffer` | ArrayBuffer to convert |

#### **Returns**

- `string` - Base64 string
