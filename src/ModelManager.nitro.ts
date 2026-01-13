import type { HybridObject } from 'react-native-nitro-modules';

/**
 * Interface for managing remote face recognition model downloads and caching.
 * Provides functionality for downloading models from CDNs like Cloudflare R2,
 * verifying checksums, and managing local cache.
 */
export interface ModelManager
  extends HybridObject<{ ios: 'swift'; android: 'kotlin' }> {
  /**
   * Download a model from a remote URL.
   * Downloads to temp location, verifies checksum, then moves to cache.
   *
   * @param url Remote URL to download model from
   * @param checksum Expected SHA-256 checksum (lowercase hex string, 64 chars)
   * @returns Promise resolving to local file path of cached model
   * @throws Error if download fails, checksum mismatch, or disk full
   *
   * @example
   * const path = await ModelManager.downloadModel(
   *   'https://r2.cloudflare.com/models/Megatron',
   *   'a3f7c8d2e1b4f9a6...'
   * );
   */
  downloadModel(url: string, checksum: string): Promise<string>;

  /**
   * Check if a model exists in cache and verify its checksum.
   * If the cached file's checksum doesn't match, it's deleted and null is returned.
   *
   * @param checksum SHA-256 checksum to look up
   * @returns Local file path if cached and valid, null otherwise
   *
   * @example
   * const cachedPath = ModelManager.getCachedModel('a3f7c8d2...');
   * if (cachedPath) {
   *   // Model is cached and valid
   * }
   */
  getCachedModel(checksum: string): string | null;
}
