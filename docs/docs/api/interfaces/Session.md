---
sidebar_position: 2
title: Session
---

# Session

Interface for managing face recognition sessions. Provides functionality for face tracking, feature extraction, and analysis.

## Methods

### `setTrackPreviewSize`

Set the track preview size in the session.

```ts
setTrackPreviewSize(size: number): void
```

#### **Parameters**

| Name   | Type     | Description                                            |
| ------ | -------- | ------------------------------------------------------ |
| `size` | `number` | Size in pixels for tracking preview. Defaults to `192` |

#### **Returns**

- `void`

---

### `setFaceDetectThreshold`

Set the face detect threshold in the session.

```ts
setFaceDetectThreshold(threshold: number): void
```

#### **Parameters**

| Name        | Type     | Description                                   |
| ----------- | -------- | --------------------------------------------- |
| `threshold` | `number` | Detection threshold value between `0` and `1` |

#### **Returns**

- `void`

---

### `setFilterMinimumFacePixelSize`

Set the minimum number of face pixels that the face detector can capture, and people below this number will be filtered.

```ts
setFilterMinimumFacePixelSize(size: number): void
```

#### **Parameters**

| Name   | Type     | Description                             |
| ------ | -------- | --------------------------------------- |
| `size` | `number` | Minimum size in pixels. Defaults to `0` |

#### **Returns**

- `void`

---

### `setTrackModeSmoothRatio`

Set the track mode smooth ratio in the session.

```ts
setTrackModeSmoothRatio(ratio: number): void
```

#### **Parameters**

| Name    | Type     | Description                                                   |
| ------- | -------- | ------------------------------------------------------------- |
| `ratio` | `number` | Smoothing ratio value between `0` and `1`. Defaults to `0.05` |

#### **Returns**

- `void`

---

### `setTrackModeNumSmoothCacheFrame`

Set the track mode num smooth cache frame in the session.

```ts
setTrackModeNumSmoothCacheFrame(num: number): void
```

#### **Parameters**

| Name  | Type     | Description                                              |
| ----- | -------- | -------------------------------------------------------- |
| `num` | `number` | Number of frames to cache for smoothing. Defaults to `5` |

#### **Returns**

- `void`

---

### `setTrackModeDetectInterval`

Set the track model detect interval in the session.

```ts
setTrackModeDetectInterval(num: number): void
```

#### **Parameters**

| Name  | Type     | Description                                         |
| ----- | -------- | --------------------------------------------------- |
| `num` | `number` | Interval value between detections. Defaults to `20` |

#### **Returns**

- `void`

---

### `executeFaceTrack`

Run face tracking in the session.

```ts
executeFaceTrack(imageStream: ImageStream): FaceData[]
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`FaceData[]`](../types/FaceData) – Array of detected face data objects.

---

### `extractFaceFeature`

Extract a face feature from a given face.

```ts
extractFaceFeature(
  imageStream: ImageStream,
  faceToken: ArrayBuffer
): ArrayBuffer
```

#### **Parameters**

| Name          | Type                                       | Description                        |
| ------------- | ------------------------------------------ | ---------------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process      |
| `faceToken`   | `ArrayBuffer`                              | Face token from previous detection |

#### **Returns**

- `ArrayBuffer` – Face feature vector representing the detected face.

---

### `getFaceAlignmentImage`

Get the face alignment image.

```ts
getFaceAlignmentImage(
  imageStream: ImageStream,
  faceToken: ArrayBuffer
): ImageBitmap
```

#### **Parameters**

| Name          | Type                                       | Description                        |
| ------------- | ------------------------------------------ | ---------------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process      |
| `faceToken`   | `ArrayBuffer`                              | Face token from previous detection |

#### **Returns**

- [`ImageBitmap`](../interfaces/ImageBitmap) – Aligned face image from the detection.

---

### `multipleFacePipelineProcess`

Process multiple faces in a pipeline.

```ts
multipleFacePipelineProcess(
  imageStream: ImageStream,
  multipleFaceData: FaceData[],
  parameter: SessionCustomParameter
): boolean
```

#### **Parameters**

| Name               | Type                                                        | Description                                  |
| ------------------ | ----------------------------------------------------------- | -------------------------------------------- |
| `imageStream`      | [`ImageStream`](../interfaces/ImageStream)                  | Input image stream to process                |
| `multipleFaceData` | [`FaceData[]`](../types/FaceData)                           | Array of face data objects to process        |
| `parameter`        | [`SessionCustomParameter`](../types/SessionCustomParameter) | Configuration for feature enabling/disabling |

#### **Returns**

- `boolean` – Returns `true` if the pipeline processing completed successfully; otherwise `false`.

---

### `getRGBLivenessConfidence`

Get the RGB liveness confidence.

```ts
getRGBLivenessConfidence(): number[]
```

#### **Returns**

- `number[]` – Confidence scores (0-1) per face.

---

### `getFaceQualityConfidence`

Get the face quality predict confidence.

```ts
getFaceQualityConfidence(): number[]
```

#### **Returns**

- `number[]` – Quality scores (0-1) per face.

---

### `getFaceMaskConfidence`

Get the face mask confidence.

```ts
getFaceMaskConfidence(): number[]
```

#### **Returns**

- `number[]` – Mask detection scores (0-1) per face.

---

### `getFaceInteractionState`

Get the prediction results of face interaction.

```ts
getFaceInteractionState(): FaceInteractionState[]
```

#### **Returns**

- [`FaceInteractionState[]`](../types/FaceInteractionState) – Array of face interaction states.

---

### `getFaceInteractionActionsResult`

Get the prediction results of face interaction actions.

```ts
getFaceInteractionActionsResult(): FaceInteractionsAction[]
```

#### **Returns**

- [`FaceInteractionsAction[]`](../types/FaceInteractionsAction) – Array of detected face interaction actions.

---

### `getFaceAttributeResult`

Get the face attribute results.

```ts
getFaceAttributeResult(): FaceAttributeResult[]
```

#### **Returns**

- [`FaceAttributeResult[]`](../types/FaceAttributeResult) – Array of detected face attributes.

---

### `getFaceEmotionResult`

Get the face emotion recognition results.

```ts
getFaceEmotionResult(): FaceEmotionResult[]
```

#### **Returns**

- [`FaceEmotionResult[]`](../types/FaceEmotionResult.md) - Array of detected face emotions

---

### `clearTrackingFace`

Clear all currently tracked faces. Useful for resetting tracking state.

```ts
clearTrackingFace(): void
```

#### **Returns**

- `void`

---

### `setTrackLostRecoveryMode`

Set the track lost recovery mode (only for LightTrack mode).

```ts
setTrackLostRecoveryMode(enable: boolean): void
```

#### **Parameters**

| Name     | Type      | Description                                     |
| -------- | --------- | ----------------------------------------------- |
| `enable` | `boolean` | Whether to enable track lost recovery (default: false) |

#### **Returns**

- `void`

---

### `setLightTrackConfidenceThreshold`

Set the light track confidence threshold (only for LightTrack mode).

```ts
setLightTrackConfidenceThreshold(value: number): void
```

#### **Parameters**

| Name    | Type     | Description                                  |
| ------- | -------- | -------------------------------------------- |
| `value` | `number` | Confidence threshold value (default: 0.1)    |

#### **Returns**

- `void`

---

### `reconfigure`

Reconfigure the session with new parameters. Internally destroys and recreates the underlying session handle. The JS object reference remains stable.

```ts
reconfigure(
  parameter: SessionCustomParameter,
  detectMode: DetectMode,
  maxDetectFaceNum: number,
  detectPixelLevel: number,
  trackByDetectModeFPS: number
): void
```

#### **Parameters**

| Name                  | Type                                                              | Description                              |
| --------------------- | ----------------------------------------------------------------- | ---------------------------------------- |
| `parameter`           | [`SessionCustomParameter`](../types/SessionCustomParameter.md)    | Custom parameters for the session        |
| `detectMode`          | [`DetectMode`](../enums/DetectMode.md)                            | Face detection mode                      |
| `maxDetectFaceNum`    | `number`                                                          | Maximum number of faces to detect        |
| `detectPixelLevel`    | `number`                                                          | Detection resolution level (-1 for default 320) |
| `trackByDetectModeFPS`| `number`                                                          | Frame rate for tracking mode (-1 for default 30) |

#### **Returns**

- `void`

### `getTrackPreviewSize`

Get the current track preview size.

```ts
getTrackPreviewSize(): number
```

#### **Returns**

- `number` - Current preview size in pixels

---

### `faceQualityDetect`

Detect the quality of a single face without running the full pipeline. Useful for quick quality checks without enabling all pipeline features.

```ts
faceQualityDetect(faceToken: ArrayBuffer): number
```

#### **Parameters**

| Name        | Type          | Description     |
| ----------- | ------------- | --------------- |
| `faceToken` | `ArrayBuffer` | Face token data |

#### **Returns**

- `number` - Quality confidence score (0-1)

---

### `extractFaceFeatureFromAlignmentImage`

Extract face features from an already-aligned face image. Use after calling `getFaceAlignmentImage()` to avoid redundant re-alignment.

```ts
extractFaceFeatureFromAlignmentImage(imageStream: ImageStream): ArrayBuffer
```

#### **Parameters**

| Name          | Type                              | Description                      |
| ------------- | --------------------------------- | -------------------------------- |
| `imageStream` | [`ImageStream`](./ImageStream.md) | Image stream of the aligned face |

#### **Returns**

- `ArrayBuffer` - Extracted face feature vector
