import { NitroModules } from 'react-native-nitro-modules';
import type { InspireFace as NitroInspireFace } from './NitroInspireFace.nitro';
export * from './NitroInspireFace.nitro';

// export const Utils = NitroModules.createHybridObject<NitroUtils>('NitroUtils');

export const InspireFace =
  NitroModules.createHybridObject<NitroInspireFace>('InspireFace');

// export const Utils = NitroModules.createHybridObject<NitroUtils>('NitroUtils');

export const BoxedInspireFace = NitroModules.box(InspireFace);
