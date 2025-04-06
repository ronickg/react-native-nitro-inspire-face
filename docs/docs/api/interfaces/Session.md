---
sidebar_position: 2
title: Session
---

# Session

Interface for managing face recognition sessions. Provides functionality for face tracking, feature extraction, and analysis.

```typescript
interface Session {
  setTrackPreviewSize(size: number): void;
  setFaceDetectThreshold(threshold: number): void;
  setFilterMinimumFacePixelSize(size: number): void;
  setTrackModeSmoothRatio(ratio: number): void;
  setTrackModeNumSmoothCacheFrame(num: number): void;
  setTrackModeDetectInterval(num: number): void;
  executeFaceTrack(imageStream: ImageStream): FaceData[];
  extractFaceFeature(
    imageStream: ImageStream,
    faceToken: ArrayBuffer
  ): ArrayBuffer;
  getFaceAlignmentImage(
    imageStream: ImageStream,
    faceToken: ArrayBuffer
  ): ImageBitmap;
  multipleFacePipelineProcess(
    imageStream: ImageStream,
    multipleFaceData: FaceData[],
    parameter: SessionCustomParameter
  ): boolean;
  getRGBLivenessConfidence(): number[];
  getFaceQualityConfidence(): number[];
  getFaceMaskConfidence(): number[];
  getFaceInteractionState(): FaceInteractionState[];
  getFaceInteractionActionsResult(): FaceInteractionsAction[];
  getFaceAttributeResult(): FaceAttributeResult[];
}
```

## Methods

### setTrackPreviewSize

```typescript
setTrackPreviewSize(size: number): void
```

Set the preview size for face tracking. Default preview size is 192 pixels.

**Parameters:**

- `size`: Size in pixels for tracking preview

### setFaceDetectThreshold

```typescript
setFaceDetectThreshold(threshold: number): void
```

Set the threshold for face detection. Controls the sensitivity of face detection.

**Parameters:**

- `threshold`: Detection threshold value

### setFilterMinimumFacePixelSize

```typescript
setFilterMinimumFacePixelSize(size: number): void
```

Set the minimum face size in pixels that the face detector can capture. Faces below this size will be filtered out.

**Parameters:**

- `size`: Minimum size in pixels (default is 0)

### setTrackModeSmoothRatio

```typescript
setTrackModeSmoothRatio(ratio: number): void
```

Set the smoothing ratio for face tracking. Default value is 0.05.

**Parameters:**

- `ratio`: Smoothing ratio value between 0 and 1

### setTrackModeNumSmoothCacheFrame

```typescript
setTrackModeNumSmoothCacheFrame(num: number): void
```

Set the number of frames used for smoothing in tracking mode. Default value is 5.

**Parameters:**

- `num`: Number of frames to cache for smoothing

### setTrackModeDetectInterval

```typescript
setTrackModeDetectInterval(num: number): void
```

Set the detection interval for tracking mode. Default value is 20.

**Parameters:**

- `num`: Interval value between detections

### executeFaceTrack

```typescript
executeFaceTrack(imageStream: ImageStream): FaceData[]
```

Execute face tracking on an image stream.

**Parameters:**

- `imageStream`: Input image stream to process

**Returns:**

- Array of detected face data including position, tracking ID, and features

### extractFaceFeature

```typescript
extractFaceFeature(
  imageStream: ImageStream,
  faceToken: ArrayBuffer
): FaceFeature
```

Extract face features from a detected face.

**Parameters:**

- `imageStream`: Input image stream
- `faceToken`: Face token data from face detection

**Returns:**

- Extracted face feature vector

### getFaceAlignmentImage

```typescript
getFaceAlignmentImage(
  imageStream: ImageStream,
  faceToken: ArrayBuffer
): ImageBitmap
```

Get the aligned face image from a detected face.

**Parameters:**

- `imageStream`: Input image stream
- `faceToken`: Face token data from face detection

**Returns:**

- Aligned face image as ImageBitmap

### multipleFacePipelineProcess

```typescript
multipleFacePipelineProcess(
  imageStream: ImageStream,
  multipleFaceData: FaceData[],
  parameter: SessionCustomParameter
): boolean
```

Process multiple faces in a pipeline, applying various face recognition and analysis features.

**Parameters:**

- `imageStream`: Input image stream
- `multipleFaceData`: Array of face data to process
- `parameter`: Custom parameters for processing

**Returns:**

- Success status of the pipeline processing

### getRGBLivenessConfidence

```typescript
getRGBLivenessConfidence(): number[]
```

Get RGB liveness detection confidence scores for detected faces.

**Returns:**

- Array of confidence scores for RGB liveness detection

### getFaceQualityConfidence

```typescript
getFaceQualityConfidence(): number[]
```

Get face quality assessment confidence scores for detected faces.

**Returns:**

- Array of confidence scores for face quality assessment

### getFaceMaskConfidence

```typescript
getFaceMaskConfidence(): number[]
```

Get face mask detection confidence scores for detected faces.

**Returns:**

- Array of confidence scores for face mask detection

### getFaceInteractionState

```typescript
getFaceInteractionState(): FaceInteractionState[]
```

Get face interaction state information for detected faces.

**Returns:**

- Array of face interaction states including eye states

### getFaceInteractionActionsResult

```typescript
getFaceInteractionActionsResult(): FaceInteractionsAction[]
```

Get face interaction action detection results for detected faces.

**Returns:**

- Array of detected face interaction actions (normal, shake, jaw open, head raise, blink)

### getFaceAttributeResult

```typescript
getFaceAttributeResult(): FaceAttributeResult[]
```

Get face attribute analysis results for detected faces.

**Returns:**

- Array of face attributes including age bracket, gender, and race predictions
