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
  ): Session;
  createImageBitmapFromFilePath(
    channels: number,
    filePath: string
  ): ImageBitmap;
  createImageStreamFromBitmap(
    bitmap: ImageBitmap,
    rotation: CameraRotation
  ): ImageStream;
}
export interface ImageStream
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  writeImageToFile(filePath: string): void;
}

export interface Session extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  setTrackPreviewSize(size: number): void;
  setFaceDetectThreshold(threshold: number): void;
  setFilterMinimumFacePixelSize(size: number): void;
  executeFaceTrack(imageStream: ImageStream): void;
}
