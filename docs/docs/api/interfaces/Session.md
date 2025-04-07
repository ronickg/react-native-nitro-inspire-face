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

### `setDetectMode`

Set the detect mode in the session.

```ts
setDetectMode(mode: number): void
```

#### **Parameters**

| Name   | Type     | Description                           |
| ------ | -------- | ------------------------------------- |
| `mode` | `number` | Detection mode value. Defaults to `0` |

#### **Returns**

- `void`

---

### `setMaxDetectFaceNum`

Set the maximum number of detected faces in the session.

```ts
setMaxDetectFaceNum(num: number): void
```

#### **Parameters**

| Name  | Type     | Description                                        |
| ----- | -------- | -------------------------------------------------- |
| `num` | `number` | Maximum number of detected faces. Defaults to `10` |

#### **Returns**

- `void`

---

### `setDetectPixelLevel`

Set the detect pixel level in the session.

```ts
setDetectPixelLevel(level: number): void
```

#### **Parameters**

| Name    | Type     | Description                                  |
| ------- | -------- | -------------------------------------------- |
| `level` | `number` | Detection pixel level value. Defaults to `0` |

#### **Returns**

- `void`

---

### `setTrackByDetectModeFPS`

Set the track by detect mode FPS in the session.

```ts
setTrackByDetectModeFPS(fps: number): void
```

#### **Parameters**

| Name  | Type     | Description                                     |
| ----- | -------- | ----------------------------------------------- |
| `fps` | `number` | Track by detect mode FPS value. Defaults to `0` |

#### **Returns**

- `void`

---

### `detectFace`

Run face detection in the session.

```ts
detectFace(imageStream: ImageStream): FaceRect[]
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`FaceRect[]`](../types/FaceRect.md) - Array of detected face rectangles

---

### `getFaceLandmark`

Get the face landmark points.

```ts
getFaceLandmark(imageStream: ImageStream): Point2f[]
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`Point2f[]`](../types/Point2f.md) - Array of facial landmark points

---

### `getFaceDenseLandmark`

Get the face dense landmark points.

```ts
getFaceDenseLandmark(imageStream: ImageStream): Point2f[]
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`Point2f[]`](../types/Point2f.md) - Array of dense facial landmark points

---

### `getFaceFeature`

Get the face feature data.

```ts
getFaceFeature(imageStream: ImageStream): ArrayBuffer
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- `ArrayBuffer` - Face feature data

---

### `getFaceQuality`

Get the face quality score.

```ts
getFaceQuality(imageStream: ImageStream): number
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- `number` - Face quality score

---

### `getFaceAngle`

Get the face angle information.

```ts
getFaceAngle(imageStream: ImageStream): FaceEulerAngle
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`FaceEulerAngle`](../types/FaceEulerAngle.md) - Face angle information

---

### `getFaceAttribute`

Get the face attribute information.

```ts
getFaceAttribute(imageStream: ImageStream): FaceAttributeResult
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- [`FaceAttributeResult`](../types/FaceAttributeResult.md) - Face attribute information

---

### `getFaceToken`

Get the face token data.

```ts
getFaceToken(imageStream: ImageStream): ArrayBuffer
```

#### **Parameters**

| Name          | Type                                       | Description                   |
| ------------- | ------------------------------------------ | ----------------------------- |
| `imageStream` | [`ImageStream`](../interfaces/ImageStream) | Input image stream to process |

#### **Returns**

- `ArrayBuffer` - Face token data

---

### `getFaceFeatureFromToken`

Get the face feature data from token.

```ts
getFaceFeatureFromToken(token: ArrayBuffer): ArrayBuffer
```

#### **Parameters**

| Name    | Type          | Description                        |
| ------- | ------------- | ---------------------------------- |
| `token` | `ArrayBuffer` | Face token from previous detection |

#### **Returns**

- `ArrayBuffer` - Face feature data from token

---

### `getFaceLandmarkFromToken`

Get the facial landmark points from token.

```ts
getFaceLandmarkFromToken(token: ArrayBuffer): Point2f[]
```

#### **Parameters**

| Name    | Type          | Description                        |
| ------- | ------------- | ---------------------------------- |
| `token` | `ArrayBuffer` | Face token from previous detection |

#### **Returns**

- [`Point2f[]`](../types/Point2f.md) - Array of facial landmark points from token

---

### `getFaceQualityFromToken`

Get the face quality score from token.

```ts
getFaceQualityFromToken(token: ArrayBuffer): number
```

#### **Parameters**

| Name    | Type          | Description                        |
| ------- | ------------- | ---------------------------------- |
| `token` | `ArrayBuffer` | Face token from previous detection |

#### **Returns**

- `number` - Face quality score from token

---

### `getFaceAngleFromToken`

Get the face angle information from token.

```ts
getFaceAngleFromToken(token: ArrayBuffer): FaceEulerAngle
```

#### **Parameters**

| Name    | Type          | Description                        |
| ------- | ------------- | ---------------------------------- |
| `token` | `ArrayBuffer` | Face token from previous detection |

#### **Returns**

- [`FaceEulerAngle`](../types/FaceEulerAngle.md) - Face angle information from token

---

### `getFaceAttributeFromToken`

Get the face attribute information from token.

```ts
getFaceAttributeFromToken(token: ArrayBuffer): FaceAttributeResult
```

#### **Parameters**

| Name    | Type          | Description                        |
| ------- | ------------- | ---------------------------------- |
| `token` | `ArrayBuffer` | Face token from previous detection |

#### **Returns**

- [`FaceAttributeResult`](../types/FaceAttributeResult.md) - Face attribute information from token
