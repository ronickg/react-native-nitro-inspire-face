import type { HybridObject } from 'react-native-nitro-modules';
import type { Color, FaceRect, Point2f, Point2i } from './types';

/**
 * Interface for handling bitmap image operations.
 * Provides functionality for image manipulation and drawing.
 */
export interface ImageBitmap
  extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  /** Width of the image in pixels */
  readonly width: number;
  /** Height of the image in pixels */
  readonly height: number;
  /** Number of color channels in the image */
  readonly channels: number;
  /** Raw image data as ArrayBuffer */
  readonly data: ArrayBuffer;

  /**
   * Draw a rectangle on the image.
   * @param rect Rectangle coordinates and dimensions
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawRect(rect: FaceRect, color: Color, thickness: number): void;

  /**
   * Draw a circle using floating-point coordinates.
   * @param point Center point of the circle
   * @param radius Radius of the circle
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawCircleF(
    point: Point2f,
    radius: number,
    color: Color,
    thickness: number
  ): void;

  /**
   * Draw a circle using integer coordinates.
   * @param point Center point of the circle
   * @param radius Radius of the circle
   * @param color RGB color for drawing
   * @param thickness Line thickness in pixels
   */
  drawCircle(
    point: Point2i,
    radius: number,
    color: Color,
    thickness: number
  ): void;
}
