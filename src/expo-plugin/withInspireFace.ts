import type { ConfigPlugin } from '@expo/config-plugins';
import { createRunOncePlugin, withPlugins } from '@expo/config-plugins';
import { withCopyAndroidModelFile } from './android/copyModelFile';
import { withIosModelFile } from './ios/copyModelFile';
import type { ConfigProps } from './@types';

const pkg = require('../../../package.json');
/**
 * Config plugin for react-native-nitro-inspire-face
 */
const withInspireFace: ConfigPlugin<ConfigProps> = (
  config,
  props = { modelName: 'Pikachu' }
) => {
  // Apply modifications to the config here
  config = withPlugins(config, [
    [withCopyAndroidModelFile, { modelName: props.modelName }],
    [withIosModelFile, { modelName: props.modelName }],
  ]);
  return config;
};

export default createRunOncePlugin(withInspireFace, pkg.name, pkg.version);
