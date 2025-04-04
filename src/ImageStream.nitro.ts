import type { HybridObject } from 'react-native-nitro-modules';
import type { CameraRotation, ImageFormat } from './enums';
import type { ImageBitmap } from './ImageBitmap.nitro';

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
