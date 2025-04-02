import type { HybridObject } from 'react-native-nitro-modules';

export interface AssetManager
  extends HybridObject<{ ios: 'swift'; android: 'kotlin' }> {
  getBaseDirectory(): string;
  copyAssetToFile(assetPath: string, filePath: string): boolean;
}
