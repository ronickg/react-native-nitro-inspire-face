import type { PrimaryKeyMode, SearchMode } from './enums';

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
 * Identity information associated with a face feature.
 * Used for face recognition and database management.
 */
export type FaceFeatureIdentity = {
  /** Unique identifier for the face feature */
  id: number;
  /** Feature vector representing the face */
  feature: ArrayBuffer;
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
