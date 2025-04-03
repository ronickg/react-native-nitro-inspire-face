import type { HybridObject } from 'react-native-nitro-modules';

/**
 * Interface for managing assets in the application.
 * Provides functionality for accessing and manipulating files and directories
 * across different platforms (iOS and Android).
 */
export interface AssetManager
  extends HybridObject<{ ios: 'swift'; android: 'kotlin' }> {
  /**
   * Get the path to the application's files directory.
   * @returns String path to the files directory
   */
  getFilesDirectory(): string;

  /**
   * Get the path to the application's databases directory.
   * @returns String path to the databases directory
   */
  getDatabasesDirectory(): string;

  /**
   * Copy an asset from the application bundle to a file in the filesystem.
   * @param assetPath Path to the asset in the application bundle
   * @param filePath Destination path in the filesystem
   * @returns Boolean indicating success or failure of the copy operation
   */
  copyAssetToFile(assetPath: string, filePath: string): boolean;
}
