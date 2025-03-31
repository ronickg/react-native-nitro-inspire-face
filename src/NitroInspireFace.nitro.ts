import type { HybridObject } from 'react-native-nitro-modules';

export enum SearchMode {
  EAGER = 0,
  EXHAUSTIVE,
}

export enum PrimaryKeyMode {
  AUTO_INCREMENT = 0,
  MANUAL_INPUT,
}

export enum DetectMode {
  ALWAYS_DETECT = 0,
  LIGHT_TRACK,
  TRACK_BY_DETECTION,
}

export enum CameraRotation {
  ROTATION_0 = 0,
  ROTATION_90,
  ROTATION_180,
  ROTATION_270,
}

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

export type ImageBitmap = {
  readonly width: number;
  readonly height: number;
  readonly channels: number;
  readonly data: ArrayBuffer;
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
export interface NitroInspireFace
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  readonly version: string;
  launch(path: string): boolean;
  featureHubDataEnable(config: FeatureHubConfiguration): boolean;
  featureHubFaceSearchThresholdSetting(threshold: number): boolean;
  createSession(
    parameter: SessionCustomParameter,
    detectMode: DetectMode,
    maxDetectFaceNum: number,
    detectPixelLevel: number,
    trackByDetectModeFPS: number
  ): NitroSession;
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
  ): NitroImageStream;
  getFaceDenseLandmarkFromFaceToken(token: ArrayBuffer): Point2f[];
  featureHubFaceInsert(feature: FaceFeatureIdentity): number;
  featureHubFaceUpdate(feature: FaceFeatureIdentity): boolean;
  featureHubFaceRemove(id: number): boolean;
  featureHubFaceSearch(feature: FaceFeature): FaceFeatureIdentity | null;
  featureHubGetFaceIdentity(id: number): FaceFeatureIdentity | null;
  featureHubFaceSearchTopK(
    feature: FaceFeature,
    topK: number
  ): SearchTopKResult[];
  getFeatureLength(): number;
  faceComparison(feature1: FaceFeature, feature2: FaceFeature): number;
}
export interface NitroImageStream
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  writeImageToFile(filePath: string): void;
}
export interface NitroSession
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  setTrackPreviewSize(size: number): void;
  setFaceDetectThreshold(threshold: number): void;
  setFilterMinimumFacePixelSize(size: number): void;
  executeFaceTrack(imageStream: NitroImageStream): FaceData[];
  extractFaceFeature(
    imageStream: NitroImageStream,
    faceToken: ArrayBuffer
  ): FaceFeature;
  multipleFacePipelineProcess(
    imageStream: NitroImageStream,
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
