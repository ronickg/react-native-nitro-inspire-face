---
sidebar_position: 1
title: InspireFace
---

# InspireFace

Main interface for the InspireFace SDK functionality. Provides comprehensive face detection, recognition, and analysis capabilities.

```typescript
interface InspireFace {
  readonly version: string;
  readonly featureLength: number;
  readonly faceDenseLandmarkLength: number;
  readonly faceBasicTokenLength: number;

  launch(path: string): void;
  reload(path: string): void;
  terminate(): void;
  featureHubDataEnable(config: FeatureHubConfiguration): void;
  featureHubDataDisable(): void;
  featureHubFaceSearchThresholdSetting(threshold: number): void;
  createSession(
    parameter: SessionCustomParameter,
    detectMode: DetectMode,
    maxDetectFaceNum: number,
    detectPixelLevel: number,
    trackByDetectModeFPS: number
  ): Session;
  createImageBitmapFromBuffer(
    buffer: ArrayBuffer,
    width: number,
    height: number,
    channels: number
  ): ImageBitmap;
  createImageBitmapFromFilePath(
    channels: number,
    filePath: string
  ): ImageBitmap;
  createImageStreamFromBitmap(
    bitmap: ImageBitmap,
    rotation: CameraRotation
  ): ImageStream;
  getFaceDenseLandmarkFromFaceToken(
    token: ArrayBuffer,
    num?: number
  ): Point2f[];
  getFaceFiveKeyPointsFromFaceToken(
    token: ArrayBuffer,
    num?: number
  ): Point2f[];
  featureHubFaceInsert(feature: FaceFeatureIdentity): number;
  featureHubFaceUpdate(feature: FaceFeatureIdentity): boolean;
  featureHubFaceRemove(id: number): boolean;
  featureHubFaceSearch(feature: ArrayBuffer): FaceFeatureIdentity | null;
  featureHubGetFaceIdentity(id: number): FaceFeatureIdentity | null;
  featureHubFaceSearchTopK(
    feature: ArrayBuffer,
    topK: number
  ): SearchTopKResult[];
  featureHubGetFaceCount(): number;
  featureHubGetExistingIds(): number[];
  faceComparison(feature1: ArrayBuffer, feature2: ArrayBuffer): number;
  getRecommendedCosineThreshold(): number;
  cosineSimilarityConvertToPercentage(similarity: number): number;
  updateCosineSimilarityConverter(config: SimilarityConverterConfig): void;
  getCosineSimilarityConverter(): SimilarityConverterConfig;
  setExpansiveHardwareRockchipDmaHeapPath(path: string): void;
  queryExpansiveHardwareRockchipDmaHeapPath(): string;
  setAppleCoreMLInferenceMode(mode: AppleCoreMLInferenceMode): void;
  setCudaDeviceId(deviceId: number): void;
  getCudaDeviceId(): number;
  printCudaDeviceInfo(): void;
  getNumCudaDevices(): number;
  checkCudaDeviceSupport(): boolean;
  fromBase64(base64: string): ArrayBuffer;
  toBase64(buffer: ArrayBuffer): string;
}
```

## Properties

### version

- Version string of the SDK.

### featureLength

- Length of face feature vectors.

### faceDenseLandmarkLength

- Number of dense facial landmarks.

### faceBasicTokenLength

- Length of basic face tokens.

## Methods

### launch

- Initializes the SDK with resources.
- Parameters:
  - `path`: `string` - Path to resource files

### reload

- Reloads the SDK with new resources.
- Parameters:
  - `path`: `string` - Path to resource files

### terminate

- Terminates the SDK and releases resources.

### featureHubDataEnable

- Enables FeatureHub data management.
- Parameters:
  - `config`: [`FeatureHubConfiguration`](../types/FeatureHubConfiguration.md) - Configuration for FeatureHub

### featureHubDataDisable

- Disables FeatureHub data management.

### featureHubFaceSearchThresholdSetting

- Sets the threshold for face search operations.
- Parameters:
  - `threshold`: `number` - Threshold value

### createSession

- Creates a new face recognition session.
- Parameters:
  - `parameter`: [`SessionCustomParameter`](../types/SessionCustomParameter.md) - Custom parameters for the session
  - `detectMode`: [`DetectMode`](../enums/DetectMode.md) - Face detection mode
  - `maxDetectFaceNum`: `number` - Maximum number of faces to detect
  - `detectPixelLevel`: `number` - Detection resolution level
  - `trackByDetectModeFPS`: `number` - Frame rate for tracking mode
- Returns: [`Session`](./Session.md) - New session instance

### createImageBitmapFromBuffer

- Creates an image bitmap from a buffer.
- Parameters:
  - `buffer`: `ArrayBuffer` - Raw image data
  - `width`: `number` - Image width
  - `height`: `number` - Image height
  - `channels`: `number` - Number of color channels
- Returns: [`ImageBitmap`](./ImageBitmap.md) - Created bitmap image

### createImageBitmapFromFilePath

- Creates an image bitmap from a file.
- Parameters:
  - `channels`: `number` - Number of color channels
  - `filePath`: `string` - Path to the image file
- Returns: [`ImageBitmap`](./ImageBitmap.md) - Created bitmap image

### createImageStreamFromBitmap

- Creates an image stream from a bitmap.
- Parameters:
  - `bitmap`: [`ImageBitmap`](./ImageBitmap.md) - Source bitmap image
  - `rotation`: [`CameraRotation`](../enums/CameraRotation.md) - Rotation to apply
- Returns: [`ImageStream`](./ImageStream.md) - Created image stream

### getFaceDenseLandmarkFromFaceToken

- Gets dense facial landmarks from a face token.
- Parameters:
  - `token`: `ArrayBuffer` - Face token data
  - `num`: `number` (optional) - Number of landmarks to retrieve
- Returns: [`Point2f[]`](../types/Point2f.md) - Array of facial landmarks

### getFaceFiveKeyPointsFromFaceToken

- Gets five key facial points from a face token.
- Parameters:
  - `token`: `ArrayBuffer` - Face token data
  - `num`: `number` (optional) - Number of points to retrieve
- Returns: [`Point2f[]`](../types/Point2f.md) - Array of facial points

### featureHubFaceInsert

- Inserts a face feature into the database.
- Parameters:
  - `feature`: [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) - Face feature identity to insert
- Returns: `number` - ID of the inserted feature

### featureHubFaceUpdate

- Updates a face feature in the database.
- Parameters:
  - `feature`: [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) - Face feature identity to update
- Returns: `boolean` - Success status

### featureHubFaceRemove

- Removes a face feature from the database.
- Parameters:
  - `id`: `number` - ID of the face feature to remove
- Returns: `boolean` - Success status

### featureHubFaceSearch

- Searches for a matching face feature.
- Parameters:
  - `feature`: `ArrayBuffer` - Feature vector to search for
- Returns: [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | `null` - Matched feature or null

### featureHubGetFaceIdentity

- Gets a face feature by ID.
- Parameters:
  - `id`: `number` - ID of the face feature to retrieve
- Returns: [`FaceFeatureIdentity`](../types/FaceFeatureIdentity.md) | `null` - Retrieved feature or null

### featureHubFaceSearchTopK

- Searches for top K similar face features.
- Parameters:
  - `feature`: `ArrayBuffer` - Feature vector to search for
  - `topK`: `number` - Number of results to return
- Returns: [`SearchTopKResult[]`](../types/SearchTopKResult.md) - Array of search results

### featureHubGetFaceCount

- Gets the total count of face features in the database.
- Returns: `number` - Number of features

### featureHubGetExistingIds

- Gets all existing face feature IDs.
- Returns: `number[]` - Array of feature IDs

### faceComparison

- Compares two face features.
- Parameters:
  - `feature1`: `ArrayBuffer` - First feature vector
  - `feature2`: `ArrayBuffer` - Second feature vector
- Returns: `number` - Similarity score

### getRecommendedCosineThreshold

- Gets the recommended threshold for cosine similarity.
- Returns: `number` - Recommended threshold value

### cosineSimilarityConvertToPercentage

- Converts cosine similarity to percentage similarity.
- Parameters:
  - `similarity`: `number` - Cosine similarity value
- Returns: `number` - Percentage similarity

### updateCosineSimilarityConverter

- Updates the similarity converter configuration.
- Parameters:
  - `config`: [`SimilarityConverterConfig`](../types/SimilarityConverterConfig.md) - New configuration settings

### getCosineSimilarityConverter

- Gets the current similarity converter configuration.
- Returns: [`SimilarityConverterConfig`](../types/SimilarityConverterConfig.md) - Current configuration

### setExpansiveHardwareRockchipDmaHeapPath

- Sets the path for Rockchip DMA heap.
- Parameters:
  - `path`: `string` - Path to DMA heap

### queryExpansiveHardwareRockchipDmaHeapPath

- Queries the current Rockchip DMA heap path.
- Returns: `string` - Current DMA heap path

### setAppleCoreMLInferenceMode

- Sets the Apple CoreML inference mode.
- Parameters:
  - `mode`: [`AppleCoreMLInferenceMode`](../enums/AppleCoreMLInferenceMode.md) - Inference mode to use

### setCudaDeviceId

- Sets the CUDA device ID.
- Parameters:
  - `deviceId`: `number` - ID of the CUDA device

### getCudaDeviceId

- Gets the current CUDA device ID.
- Returns: `number` - Current device ID

### printCudaDeviceInfo

- Prints information about the CUDA device.

### getNumCudaDevices

- Gets the number of available CUDA devices.
- Returns: `number` - Number of devices

### checkCudaDeviceSupport

- Checks if CUDA device support is available.
- Returns: `boolean` - Support status

### fromBase64

- Converts a base64 string to an ArrayBuffer.
- Parameters:
  - `base64`: `string` - Base64 string to convert
- Returns: `ArrayBuffer` - Converted data

### toBase64

- Converts an ArrayBuffer to a base64 string.
- Parameters:
  - `buffer`: `ArrayBuffer` - ArrayBuffer to convert
- Returns: `string` - Base64 string
