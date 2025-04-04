import type { HybridObject } from 'react-native-nitro-modules';
import type { ImageStream } from './ImageStream.nitro';
import type { ImageBitmap } from './ImageBitmap.nitro';
import type {
  FaceData,
  FaceFeature,
  FaceInteractionState,
  SessionCustomParameter,
  FaceAttributeResult,
  FaceInteractionsAction,
} from './types';

/**
 * Interface for managing face recognition sessions.
 * Provides functionality for face tracking, feature extraction, and analysis.
 */
export interface Session extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  /**
   * Set the preview size for face tracking.
   * @param size Size in pixels
   */
  setTrackPreviewSize(size: number): void;

  /**
   * Set the threshold for face detection.
   * @param threshold Detection threshold value
   */
  setFaceDetectThreshold(threshold: number): void;

  /**
   * Set the minimum face size in pixels for detection.
   * @param size Minimum size in pixels
   */
  setFilterMinimumFacePixelSize(size: number): void;

  /**
   * Set the smoothing ratio for face tracking.
   * @param ratio Smoothing ratio value
   */
  setTrackModeSmoothRatio(ratio: number): void;

  /**
   * Set the number of frames for smoothing in tracking mode.
   * @param num Number of frames
   */
  setTrackModeNumSmoothCacheFrame(num: number): void;

  /**
   * Set the detection interval for tracking mode.
   * @param num Interval value
   */
  setTrackModeDetectInterval(num: number): void;

  /**
   * Execute face tracking on an image stream.
   * @param imageStream Input image stream
   */
  executeFaceTrack(imageStream: ImageStream): FaceData[];

  /**
   * Extract face features from a detected face.
   * @param imageStream Input image stream
   * @param faceToken Face token data
   */
  extractFaceFeature(
    imageStream: ImageStream,
    faceToken: ArrayBuffer
  ): FaceFeature;

  /**
   * Get the aligned face image.
   * @param imageStream Input image stream
   * @param faceToken Face token data
   */
  getFaceAlignmentImage(
    imageStream: ImageStream,
    faceToken: ArrayBuffer
  ): ImageBitmap;

  /**
   * Process multiple faces in a pipeline.
   * @param imageStream Input image stream
   * @param multipleFaceData Data for multiple faces
   * @param parameter Custom parameters for processing
   */
  multipleFacePipelineProcess(
    imageStream: ImageStream,
    multipleFaceData: FaceData[],
    parameter: SessionCustomParameter
  ): boolean;

  /**
   * Get RGB liveness detection confidence scores.
   */
  getRGBLivenessConfidence(): number[];

  /**
   * Get face quality assessment confidence scores.
   */
  getFaceQualityConfidence(): number[];

  /**
   * Get face mask detection confidence scores.
   */
  getFaceMaskConfidence(): number[];

  /**
   * Get face interaction state information.
   */
  getFaceInteractionState(): FaceInteractionState[];

  /**
   * Get face interaction action detection results.
   */
  getFaceInteractionActionsResult(): FaceInteractionsAction[];

  /**
   * Get face attribute analysis results.
   */
  getFaceAttributeResult(): FaceAttributeResult[];
}
