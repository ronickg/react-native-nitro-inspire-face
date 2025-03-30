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
  enableRecognition: boolean;
  enableLiveness: boolean;
  enableIrLiveness: boolean;
  enableMaskDetect: boolean;
  enableFaceQuality: boolean;
  enableFaceAttribute: boolean;
  enableInteractionLiveness: boolean;
  enableDetectModeLandmark: boolean;
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

export type FaceBasicToken = {
  size: number;
  data: ArrayBuffer;
};

export type MultipleFaceData = {
  detectedNum: number;
  rects: FaceRect[];
  trackIds: number[];
  detConfidence: number[];
  angles: FaceEulerAngle;
  tokens: FaceBasicToken[];
};

export type Point2f = {
  x: number;
  y: number;
};

export type FaceFeature = {
  size: number;
  data: ArrayBuffer;
};

export interface NitroInspireFace
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  readonly version: string;
  multiply(a: number, b: number): number;
  launch(path: string): void;
  featureHubDataEnable(config: FeatureHubConfiguration): void;
  featureHubFaceSearchThresholdSetting(threshold: number): void;
  createSession(
    parameter: SessionCustomParameter,
    detectMode: DetectMode,
    maxDetectFaceNum: number,
    detectPixelLevel: number,
    trackByDetectModeFPS: number
  ): NitroSession;
  createImageBitmapFromFilePath(
    channels: number,
    filePath: string
  ): ImageBitmap;
  createImageStreamFromBitmap(
    bitmap: ImageBitmap,
    rotation: CameraRotation
  ): NitroImageStream;
  getFaceDenseLandmarkFromFaceToken(token: FaceBasicToken): Point2f[];
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
  executeFaceTrack(imageStream: NitroImageStream): MultipleFaceData;
  extractFaceFeature(
    imageStream: NitroImageStream,
    faceToken: FaceBasicToken
  ): FaceFeature;
}
