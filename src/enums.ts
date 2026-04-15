/**
 * Select the search mode in the process of face recognition search.
 * Different modes will affect the execution efficiency and results.
 */
export enum SearchMode {
  /**
   * Eager mode: Stops when a vector meets the threshold.
   */
  EAGER = 0,

  /**
   * Exhaustive mode: Searches until the best match is found.
   */
  EXHAUSTIVE = 1,
}

/**
 * Primary key mode for face feature management.
 */
export enum PrimaryKeyMode {
  /**
   * Auto-increment mode for primary key.
   */
  AUTO_INCREMENT = 0,

  /**
   * Manual input mode for primary key.
   */
  MANUAL_INPUT = 1,
}

/**
 * Apple CoreML inference mode.
 */
export enum AppleCoreMLInferenceMode {
  /**
   * CPU Only.
   */
  CPU = 0,

  /**
   * GPU first.
   */
  GPU = 1,

  /**
   * Automatic selection, ANE first.
   */
  ANE = 2,
}

/**
 * Face detection modes for different scenarios.
 */
export enum DetectMode {
  /**
   * Image detection mode, always detect, applicable to images.
   */
  ALWAYS_DETECT = 0,

  /**
   * Video detection mode, face tracking, applicable to video streaming, front camera.
   */
  LIGHT_TRACK = 1,

  /**
   * Video detection mode, face tracking, applicable to high resolution, monitoring, capturing.
   * (You need a specific option turned on at compile time to use it).
   */
  TRACK_BY_DETECTION = 2,
}

/**
 * Camera picture rotation mode.
 * To accommodate the rotation of certain devices, four image rotation modes are provided.
 */
export enum CameraRotation {
  /**
   * 0 degree rotation.
   */
  ROTATION_0 = 0,

  /**
   * 90 degree rotation.
   */
  ROTATION_90 = 1,

  /**
   * 180 degree rotation.
   */
  ROTATION_180 = 2,

  /**
   * 270 degree rotation.
   */
  ROTATION_270 = 3,
}

/**
 * Camera stream format.
 * Contains several common camera stream formats available in the market.
 */
export enum ImageFormat {
  /**
   * Image in RGB format.
   */
  RGB = 0,

  /**
   * Image in BGR format (Opencv Mat default).
   */
  BGR = 1,

  /**
   * Image in RGB format with alpha channel.
   */
  RGBA = 2,

  /**
   * Image in BGR format with alpha channel.
   */
  BGRA = 3,

  /**
   * Image in YUV NV12 format.
   */
  YUV_NV12 = 4,

  /**
   * Image in YUV NV21 format.
   */
  YUV_NV21 = 5,

  /**
   * Image in I420 format.
   */
  I420 = 6,

  /**
   * Image in GRAY format.
   */
  GRAY = 7,
}

/**
 * Landmark engine options for face landmark detection.
 * Different engines offer different accuracy/speed tradeoffs.
 */
export enum LandmarkEngine {
  /**
   * HypLMv2 0.25 - lightweight, fastest (default).
   */
  HYPLMV2_025 = 0,

  /**
   * HypLMv2 0.50 - balanced accuracy and speed.
   */
  HYPLMV2_050 = 1,

  /**
   * InsightFace 2D106 track - highest accuracy, heavier.
   */
  INSIGHTFACE_2D106 = 2,
}

/**
 * SDK built-in log level mode.
 */
export enum InspireFaceLogLevel {
  /**
   * No logging, disables all log output.
   */
  LOG_NONE = 0,

  /**
   * Debug level for detailed system information mostly useful for developers.
   */
  LOG_DEBUG = 1,

  /**
   * Information level for general system information about operational status.
   */
  LOG_INFO = 2,

  /**
   * Warning level for non-critical issues that might need attention.
   */
  LOG_WARN = 3,

  /**
   * Error level for error events that might still allow the application to continue running.
   */
  LOG_ERROR = 4,

  /**
   * Fatal level for severe error events that will presumably lead the application to abort.
   */
  LOG_FATAL = 5,
}
