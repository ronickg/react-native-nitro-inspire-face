import type { HybridObject } from 'react-native-nitro-modules';
import type { ImageStream } from './ImageStream.nitro';
import type { ImageBitmap } from './ImageBitmap.nitro';
import type { DetectMode } from './enums';
import type {
  FaceData,
  FaceEmotionResult,
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
   * Set the track lost recovery mode (only for LightTrack mode).
   * @param enable Whether to enable track lost recovery (default: false)
   */
  setTrackLostRecoveryMode(enable: boolean): void;

  /**
   * Set the light track confidence threshold (only for LightTrack mode).
   * @param value Confidence threshold value (default: 0.1)
   */
  setLightTrackConfidenceThreshold(value: number): void;

  /**
   * Clear all currently tracked faces.
   */
  clearTrackingFace(): void;

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
  ): ArrayBuffer;

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

  /**
   * Get face emotion recognition results.
   */
  getFaceEmotionResult(): FaceEmotionResult[];

  /**
   * Get the current track preview size.
   */
  getTrackPreviewSize(): number;

  /**
   * Detect the quality of a single face without running the full pipeline.
   * @param faceToken Face token data
   */
  faceQualityDetect(faceToken: ArrayBuffer): number;

  /**
   * Extract face features from an already-aligned face image.
   * Use after calling getFaceAlignmentImage() to avoid re-alignment.
   * @param imageStream Image stream of the aligned face
   */
  extractFaceFeatureFromAlignmentImage(imageStream: ImageStream): ArrayBuffer;

  /**
   * Reconfigure the session with new parameters.
   * Internally destroys and recreates the underlying session handle.
   * The JS object reference remains stable.
   * @param parameter Custom parameters for the session
   * @param detectMode Face detection mode
   * @param maxDetectFaceNum Maximum number of faces to detect
   * @param detectPixelLevel Detection resolution level (-1 for default 320)
   * @param trackByDetectModeFPS Frame rate for tracking mode (-1 for default 30)
   */
  reconfigure(
    parameter: SessionCustomParameter,
    detectMode: DetectMode,
    maxDetectFaceNum: number,
    detectPixelLevel: number,
    trackByDetectModeFPS: number
  ): void;
}
