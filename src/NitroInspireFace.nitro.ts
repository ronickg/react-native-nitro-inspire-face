import type { HybridObject } from 'react-native-nitro-modules';
import type {
  AppleCoreMLInferenceMode,
  CameraRotation,
  DetectMode,
  ImageFormat,
  PrimaryKeyMode,
  SearchMode,
} from './types';

export type SessionCustomParameter = {
  enableRecognition?: boolean;
  enableLiveness?: boolean;
  enableIrLiveness?: boolean;
  enableMaskDetect?: boolean;
  enableFaceQuality?: boolean;
  enableFaceAttribute?: boolean;
  enableInteractionLiveness?: boolean;
  enableDetectModeLandmark?: boolean;
};

export type FeatureHubConfiguration = {
  searchMode: SearchMode;
  enablePersistence: boolean;
  persistenceDbPath: string;
  searchThreshold: number;
  primaryKeyMode: PrimaryKeyMode;
};

export type FaceRect = {
  x: number;
  y: number;
  width: number;
  height: number;
};

export type FaceEulerAngle = {
  roll: number;
  yaw: number;
  pitch: number;
};

export type FaceData = {
  rect: FaceRect;
  trackId: number;
  detConfidence: number;
  angle: FaceEulerAngle;
  token: ArrayBuffer;
};

export type Point2f = {
  x: number;
  y: number;
};

export type Point2i = {
  x: number;
  y: number;
};

export type Color = {
  r: number;
  g: number;
  b: number;
};

export type FaceFeature = number[];

export type FaceFeatureIdentity = {
  id: number;
  feature: FaceFeature;
  confidence?: number;
};

export type SearchTopKResult = {
  confidence: number;
  id: number;
};

export type FaceInteractionState = {
  left: number;
  right: number;
};

export type FaceInteractionsAction = {
  normal: number;
  shake: number;
  jawOpen: number;
  headRaise: number;
  blink: number;
};

export type FaceAttributeResult = {
  ageBracket: number;
  gender: number;
  race: number;
};

export type SimilarityConverterConfig = {
  threshold: number;
  middleScore: number;
  steepness: number;
  outputMin: number;
  outputMax: number;
};

export interface ImageBitmap
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  readonly width: number;
  readonly height: number;
  readonly channels: number;
  readonly data: ArrayBuffer;
  // writeToFile(filePath: string): void; // TODO: Seems like this function is not working
  drawRect(rect: FaceRect, color: Color, thickness: number): void;
  drawCircleF(
    point: Point2f,
    radius: number,
    color: Color,
    thickness: number
  ): void;
  drawCircle(
    point: Point2i,
    radius: number,
    color: Color,
    thickness: number
  ): void;
}

export interface ImageStream
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  writeImageToFile(filePath: string): void;
  setFormat(format: ImageFormat): void;
  setRotation(rotation: CameraRotation): void;
  createImageBitmap(isRotate?: boolean, scale?: number): ImageBitmap;
}
export interface InspireFace
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
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
  featureHubFaceSearch(feature: FaceFeature): FaceFeatureIdentity | null;
  featureHubGetFaceIdentity(id: number): FaceFeatureIdentity | null;
  featureHubFaceSearchTopK(
    feature: FaceFeature,
    topK: number
  ): SearchTopKResult[];
  featureHubGetFaceCount(): number;
  featureHubGetExistingIds(): number[];
  faceComparison(feature1: FaceFeature, feature2: FaceFeature): number;
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
}
export interface Session extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
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
  ): FaceFeature;
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
