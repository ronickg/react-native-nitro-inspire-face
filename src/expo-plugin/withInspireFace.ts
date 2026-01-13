import type { ConfigPlugin } from '@expo/config-plugins';
import { createRunOncePlugin, withPlugins } from '@expo/config-plugins';
import { withCopyAndroidModelFile } from './android/copyModelFile';
import { withIosModelFile } from './ios/copyModelFile';
import type { ConfigProps } from './@types';

const pkg = require('../../../package.json');
/**
 * Config plugin for react-native-nitro-inspire-face
 */
const withInspireFace: ConfigPlugin<ConfigProps> = (config, props = {}) => {
  // Only apply model copying plugins if modelName is provided
  // This allows users who use launchWithRemoteModel() to skip bundling models
  if (props.modelName) {
    config = withPlugins(config, [
      [
        withCopyAndroidModelFile,
        { modelName: props.modelName, modelDir: props.modelDir },
      ],
      [
        withIosModelFile,
        { modelName: props.modelName, modelDir: props.modelDir },
      ],
    ]);
  }
  return config;
};

export default createRunOncePlugin(withInspireFace, pkg.name, pkg.version);
