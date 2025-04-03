import type { HybridObject } from 'react-native-nitro-modules';
import type {
  AppleCoreMLInferenceMode,
  CameraRotation,
  DetectMode,
  ImageFormat,
  PrimaryKeyMode,
  SearchMode,
} from './types';

/**
 * Custom parameters for configuring a face recognition session.
 * This type holds various flags to enable or disable specific features
 * in the face recognition context, such as face recognition, liveness detection,
 * mask detection, quality assessment, and attribute prediction.
 */
export type SessionCustomParameter = {
  /** Enable face recognition feature */
  enableRecognition?: boolean;
  /** Enable RGB liveness detection feature */
  enableLiveness?: boolean;
  /** Enable IR (Infrared) liveness detection feature */
  enableIrLiveness?: boolean;
  /** Enable mask detection feature */
  enableMaskDetect?: boolean;
  /** Enable face quality assessment feature */
  enableFaceQuality?: boolean;
  /** Enable face attribute prediction feature */
  enableFaceAttribute?: boolean;
  /** Enable interaction for liveness detection feature */
  enableInteractionLiveness?: boolean;
  /** Enable landmark detection in detection mode */
  enableDetectModeLandmark?: boolean;
};

/**
 * Configuration settings for the FeatureHub functionality.
 * FeatureHub is a built-in global lightweight face feature vector management system.
 * It supports basic face feature search, deletion, and modification functions,
 * and offers two optional data storage modes: in-memory and persistence.
 */
export type FeatureHubConfiguration = {
  /** Mode of face search affecting execution efficiency and results */
  searchMode: SearchMode;
  /** Flag to enable or disable data persistence */
  enablePersistence: boolean;
  /** Path to the database file for persistence storage */
  persistenceDbPath: string;
  /** Threshold value for face search comparisons */
  searchThreshold: number;
  /** Mode for managing primary keys in the database */
  primaryKeyMode: PrimaryKeyMode;
};

/**
 * Rectangle representing a face region in an image.
 * Defines the position and size of a detected face.
 */
export type FaceRect = {
  /** X-coordinate of the top-left corner of the rectangle */
  x: number;
  /** Y-coordinate of the top-left corner of the rectangle */
  y: number;
  /** Width of the rectangle */
  width: number;
  /** Height of the rectangle */
  height: number;
};

/**
 * Euler angles representing the orientation of a face in 3D space.
 * These angles describe the rotation of the face along different axes.
 */
export type FaceEulerAngle = {
  /** Roll angle of the face (rotation around the Z-axis) */
  roll: number;
  /** Yaw angle of the face (rotation around the Y-axis) */
  yaw: number;
  /** Pitch angle of the face (rotation around the X-axis) */
  pitch: number;
};

/**
 * Comprehensive data structure for a detected face.
 * Contains information about the face's position, tracking, confidence,
 * orientation, and feature token.
 */
export type FaceData = {
  /** Rectangle defining the face region */
  rect: FaceRect;
  /** Unique identifier for tracking the face across frames */
  trackId: number;
  /** Confidence score of the face detection */
  detConfidence: number;
  /** 3D orientation of the face */
  angle: FaceEulerAngle;
  /** Binary token containing face feature data */
  token: ArrayBuffer;
};

/**
 * 2D point with floating-point coordinates.
 * Used for precise positioning of facial features.
 */
export type Point2f = {
  /** X-coordinate as a floating-point number */
  x: number;
  /** Y-coordinate as a floating-point number */
  y: number;
};

/**
 * 2D point with integer coordinates.
 * Used for pixel-precise positioning.
 */
export type Point2i = {
  /** X-coordinate as an integer */
  x: number;
  /** Y-coordinate as an integer */
  y: number;
};

/**
 * RGB color representation.
 * Used for drawing and visualization purposes.
 */
export type Color = {
  /** Red component (0-255) */
  r: number;
  /** Green component (0-255) */
  g: number;
  /** Blue component (0-255) */
  b: number;
};

/**
 * Array of numbers representing face feature vector.
 * Used in face recognition and comparison operations.
 */
export type FaceFeature = number[];

/**
 * Identity information associated with a face feature.
 * Used for face recognition and database management.
 */
export type FaceFeatureIdentity = {
  /** Unique identifier for the face feature */
  id: number;
  /** Feature vector representing the face */
  feature: FaceFeature;
  /** Optional confidence score for the feature */
  confidence?: number;
};

/**
 * Result of a top-K search operation in face recognition.
 * Contains the confidence score and ID of a matched face.
 */
export type SearchTopKResult = {
  /** Confidence score of the match */
  confidence: number;
  /** ID of the matched face */
  id: number;
};

/**
 * State information for face interaction detection.
 * Used to track the state of eyes during interaction.
 */
export type FaceInteractionState = {
  /** State of the left eye (0-1, where 1 means fully open) */
  left: number;
  /** State of the right eye (0-1, where 1 means fully open) */
  right: number;
};

/**
 * Detection results for various face interaction actions.
 * Used to track different types of facial movements and gestures.
 */
export type FaceInteractionsAction = {
  /** Detection score for normal face state */
  normal: number;
  /** Detection score for head shake action */
  shake: number;
  /** Detection score for jaw opening action */
  jawOpen: number;
  /** Detection score for head raising action */
  headRaise: number;
  /** Detection score for eye blinking action */
  blink: number;
};

/**
 * Results of face attribute analysis.
 * Contains predictions for age, gender, and race.
 */
export type FaceAttributeResult = {
  /** Age bracket prediction (0-8, representing different age ranges) */
  ageBracket: number;
  /** Gender prediction (0: Female, 1: Male) */
  gender: number;
  /** Race prediction (0: Black, 1: Asian, 2: Latino/Hispanic, 3: Middle Eastern, 4: White) */
  race: number;
};

/**
 * Configuration for similarity score conversion.
 * Used to convert cosine similarity scores to percentage-based similarity.
 */
export type SimilarityConverterConfig = {
  /** Threshold for determining similarity */
  threshold: number;
  /** Reference value for percentage conversion */
  middleScore: number;
  /** Steepness of the conversion curve */
  steepness: number;
  /** Minimum output value */
  outputMin: number;
  /** Maximum output value */
  outputMax: number;
};

/**
 * Interface for handling bitmap image operations.
 * Provides functionality for image manipulation and drawing.
 */
export interface ImageBitmap
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  /** Width of the image in pixels */
  readonly width: number;
  /** Height of the image in pixels */
  readonly height: number;
  /** Number of color channels in the image */
  readonly channels: number;
  /** Raw image data as ArrayBuffer */
  readonly data: ArrayBuffer;

  /**
   * Draw a rectangle on the image.
   * @param rect Rectangle coordinates and dimensions
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawRect(rect: FaceRect, color: Color, thickness: number): void;

  /**
   * Draw a circle using floating-point coordinates.
   * @param point Center point of the circle
   * @param radius Radius of the circle
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawCircleF(
    point: Point2f,
    radius: number,
    color: Color,
    thickness: number
  ): void;

  /**
   * Draw a circle using integer coordinates.
   * @param point Center point of the circle
   * @param radius Radius of the circle
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawCircle(
    point: Point2i,
    radius: number,
    color: Color,
    thickness: number
  ): void;
}

/**
 * Interface for handling image stream operations.
 * Provides functionality for image stream manipulation and conversion.
 */
export interface ImageStream
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  /**
   * Write the image stream to a file.
   * @param filePath Path where the image will be saved
   */
  writeImageToFile(filePath: string): void;

  /**
   * Set the format of the image stream.
   * @param format Image format to use
   */
  setFormat(format: ImageFormat): void;

  /**
   * Set the rotation of the image stream.
   * @param rotation Rotation angle to apply
   */
  setRotation(rotation: CameraRotation): void;

  /**
   * Create a bitmap image from the stream.
   * @param isRotate Whether to apply rotation
   * @param scale Scale factor to apply
   */
  createImageBitmap(isRotate?: boolean, scale?: number): ImageBitmap;
}

/**
 * Main interface for the InspireFace SDK functionality.
 * Provides comprehensive face detection, recognition, and analysis capabilities.
 */
export interface InspireFace
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  /** Version string of the SDK */
  readonly version: string;
  /** Length of face feature vectors */
  readonly featureLength: number;
  /** Number of dense facial landmarks */
  readonly faceDenseLandmarkLength: number;
  /** Length of basic face tokens */
  readonly faceBasicTokenLength: number;

  /**
   * Initialize the SDK with resources.
   * @param path Path to resource files
   */
  launch(path: string): void;

  /**
   * Reload the SDK with new resources.
   * @param path Path to resource files
   */
  reload(path: string): void;

  /**
   * Terminate the SDK and release resources.
   */
  terminate(): void;

  /**
   * Enable FeatureHub data management.
   * @param config Configuration for FeatureHub
   */
  featureHubDataEnable(config: FeatureHubConfiguration): void;

  /**
   * Disable FeatureHub data management.
   */
  featureHubDataDisable(): void;

  /**
   * Set the threshold for face search operations.
   * @param threshold Threshold value
   */
  featureHubFaceSearchThresholdSetting(threshold: number): void;

  /**
   * Create a new face recognition session.
   * @param parameter Custom parameters for the session
   * @param detectMode Face detection mode
   * @param maxDetectFaceNum Maximum number of faces to detect
   * @param detectPixelLevel Detection resolution level
   * @param trackByDetectModeFPS Frame rate for tracking mode
   */
  createSession(
    parameter: SessionCustomParameter,
    detectMode: DetectMode,
    maxDetectFaceNum: number,
    detectPixelLevel: number,
    trackByDetectModeFPS: number
  ): Session;

  /**
   * Create an image bitmap from a buffer.
   * @param buffer Raw image data
   * @param width Image width
   * @param height Image height
   * @param channels Number of color channels
   */
  createImageBitmapFromBuffer(
    buffer: ArrayBuffer,
    width: number,
    height: number,
    channels: number
  ): ImageBitmap;

  /**
   * Create an image bitmap from a file.
   * @param channels Number of color channels
   * @param filePath Path to the image file
   */
  createImageBitmapFromFilePath(
    channels: number,
    filePath: string
  ): ImageBitmap;

  /**
   * Create an image stream from a bitmap.
   * @param bitmap Source bitmap image
   * @param rotation Rotation to apply
   */
  createImageStreamFromBitmap(
    bitmap: ImageBitmap,
    rotation: CameraRotation
  ): ImageStream;

  /**
   * Get dense facial landmarks from a face token.
   * @param token Face token data
   * @param num Optional number of landmarks to retrieve
   */
  getFaceDenseLandmarkFromFaceToken(
    token: ArrayBuffer,
    num?: number
  ): Point2f[];

  /**
   * Get five key facial points from a face token.
   * @param token Face token data
   * @param num Optional number of points to retrieve
   */
  getFaceFiveKeyPointsFromFaceToken(
    token: ArrayBuffer,
    num?: number
  ): Point2f[];

  /**
   * Insert a face feature into the database.
   * @param feature Face feature identity to insert
   */
  featureHubFaceInsert(feature: FaceFeatureIdentity): number;

  /**
   * Update a face feature in the database.
   * @param feature Face feature identity to update
   */
  featureHubFaceUpdate(feature: FaceFeatureIdentity): boolean;

  /**
   * Remove a face feature from the database.
   * @param id ID of the face feature to remove
   */
  featureHubFaceRemove(id: number): boolean;

  /**
   * Search for a matching face feature.
   * @param feature Feature vector to search for
   */
  featureHubFaceSearch(feature: FaceFeature): FaceFeatureIdentity | null;

  /**
   * Get a face feature by ID.
   * @param id ID of the face feature to retrieve
   */
  featureHubGetFaceIdentity(id: number): FaceFeatureIdentity | null;

  /**
   * Search for top K similar face features.
   * @param feature Feature vector to search for
   * @param topK Number of results to return
   */
  featureHubFaceSearchTopK(
    feature: FaceFeature,
    topK: number
  ): SearchTopKResult[];

  /**
   * Get the total count of face features in the database.
   */
  featureHubGetFaceCount(): number;

  /**
   * Get all existing face feature IDs.
   */
  featureHubGetExistingIds(): number[];

  /**
   * Compare two face features.
   * @param feature1 First feature vector
   * @param feature2 Second feature vector
   */
  faceComparison(feature1: FaceFeature, feature2: FaceFeature): number;

  /**
   * Get the recommended threshold for cosine similarity.
   */
  getRecommendedCosineThreshold(): number;

  /**
   * Convert cosine similarity to percentage similarity.
   * @param similarity Cosine similarity value
   */
  cosineSimilarityConvertToPercentage(similarity: number): number;

  /**
   * Update the similarity converter configuration.
   * @param config New configuration settings
   */
  updateCosineSimilarityConverter(config: SimilarityConverterConfig): void;

  /**
   * Get the current similarity converter configuration.
   */
  getCosineSimilarityConverter(): SimilarityConverterConfig;

  /**
   * Set the path for Rockchip DMA heap.
   * @param path Path to DMA heap
   */
  setExpansiveHardwareRockchipDmaHeapPath(path: string): void;

  /**
   * Query the current Rockchip DMA heap path.
   */
  queryExpansiveHardwareRockchipDmaHeapPath(): string;

  /**
   * Set the Apple CoreML inference mode.
   * @param mode Inference mode to use
   */
  setAppleCoreMLInferenceMode(mode: AppleCoreMLInferenceMode): void;

  /**
   * Set the CUDA device ID.
   * @param deviceId ID of the CUDA device
   */
  setCudaDeviceId(deviceId: number): void;

  /**
   * Get the current CUDA device ID.
   */
  getCudaDeviceId(): number;

  /**
   * Print information about the CUDA device.
   */
  printCudaDeviceInfo(): void;

  /**
   * Get the number of available CUDA devices.
   */
  getNumCudaDevices(): number;

  /**
   * Check if CUDA device support is available.
   */
  checkCudaDeviceSupport(): boolean;
}

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
