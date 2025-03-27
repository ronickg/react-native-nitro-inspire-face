import { NitroModules } from 'react-native-nitro-modules';
import type { NitroInspireFace } from './NitroInspireFace.nitro';

const NitroInspireFaceHybridObject =
  NitroModules.createHybridObject<NitroInspireFace>('NitroInspireFace');

export function multiply(a: number, b: number): number {
  return NitroInspireFaceHybridObject.multiply(a, b);
}
