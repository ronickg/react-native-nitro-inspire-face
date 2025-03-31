import { NitroModules } from 'react-native-nitro-modules';
import type { NitroInspireFace } from './NitroInspireFace.nitro';
export * from './NitroInspireFace.nitro';

export const InspireFace =
  NitroModules.createHybridObject<NitroInspireFace>('NitroInspireFace');

export const BoxedInspireFace = NitroModules.box(InspireFace);
