import type { HybridObject } from 'react-native-nitro-modules';
import type {
  AppleCoreMLInferenceMode,
  CameraRotation,
  DetectMode,
} from './enums';
import type { ImageBitmap } from './ImageBitmap.nitro';
import type { ImageStream } from './ImageStream.nitro';
import type { Session } from './Session.nitro';
import type {
  FaceFeatureIdentity,
  FeatureHubConfiguration,
  Point2f,
  SearchTopKResult,
  SessionCustomParameter,
  SimilarityConverterConfig,
} from './types';

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
   * Automatically detects if path is a bundled asset name or absolute file path.
   *
   * @param path Either:
   *   - Asset name (e.g., 'Pikachu') - copies from bundle to filesystem
   *   - Absolute path (e.g., '/path/to/model.tar') - uses directly
   *
   * @example
   * // Bundled model
   * InspireFace.launch('Pikachu');
   *
   * @example
   * // Downloaded model
   * const modelPath = await ModelManager.downloadModel(url, checksum);
   * InspireFace.launch(modelPath);
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
  featureHubFaceSearch(feature: ArrayBuffer): FaceFeatureIdentity | null;

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
    feature: ArrayBuffer,
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
  faceComparison(feature1: ArrayBuffer, feature2: ArrayBuffer): number;

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

  /**
   * Convert a base64 string to an ArrayBuffer.
   * @param base64 Base64 string to convert
   */
  fromBase64(base64: string): ArrayBuffer;

  /**
   * Convert an ArrayBuffer to a base64 string.
   * @param buffer ArrayBuffer to convert
   */
  toBase64(buffer: ArrayBuffer): string;
}
