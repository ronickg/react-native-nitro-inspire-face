import { NitroModules } from 'react-native-nitro-modules';
import type { InspireFace as NitroInspireFace } from './InspireFace.nitro';
import type { AssetManager as NitroAssetManager } from './AssetManager.nitro';
import type { ModelManager as NitroModelManager } from './ModelManager.nitro';
export * from './InspireFace.nitro';
export * from './Session.nitro';
export * from './ImageStream.nitro';
export * from './ImageBitmap.nitro';
export * from './AssetManager.nitro';
export * from './ModelManager.nitro';
export * from './enums';
export * from './types';

export const InspireFace =
  NitroModules.createHybridObject<NitroInspireFace>('InspireFace');

export const AssetManager =
  NitroModules.createHybridObject<NitroAssetManager>('AssetManager');

export const ModelManager =
  NitroModules.createHybridObject<NitroModelManager>('ModelManager');

export const BoxedInspireFace = NitroModules.box(InspireFace);

/**
 * Launch InspireFace with a remote model.
 * Downloads and caches the model, then launches the SDK.
 *
 * @param url Remote model URL (e.g. https://github.com/HyperInspire/InspireFace/releases/download/v1.x/Pikachu)
 * @param checksum SHA-256 checksum of the model file (lowercase hex string, 64 chars)
 * @returns Promise that resolves when the model is loaded and SDK is launched
 * @throws Error if download fails, checksum mismatch, or SDK launch fails
 *
 * @example
 * await launchWithRemoteModel(
 *   'https://github.com/HyperInspire/InspireFace/releases/download/v1.x/Pikachu',
 *   '5037ba1f49905b783a1c973d5d58b834a645922cc2814c8e3ca630a38dc24431'
 * );
 */
export async function launchWithRemoteModel(
  url: string,
  checksum: string
): Promise<void> {
  const cachedPath = ModelManager.getCachedModel(checksum);

  if (cachedPath) {
    console.log('[launchWithRemoteModel] Using cached model:', cachedPath);
    InspireFace.launch(cachedPath);
    return;
  }

  console.log('[launchWithRemoteModel] Downloading model from:', url);
  const downloadedPath = await ModelManager.downloadModel(url, checksum);

  console.log('[launchWithRemoteModel] Launching with model:', downloadedPath);
  InspireFace.launch(downloadedPath);
}
