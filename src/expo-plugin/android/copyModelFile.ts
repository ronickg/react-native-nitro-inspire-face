import type { ConfigPlugin } from '@expo/config-plugins';
import { withDangerousMod } from '@expo/config-plugins';

import path from 'path';
import fs from 'fs';
import type { ConfigProps } from '../@types';

/**
 * Copy model file to Android assets
 */
export const withCopyAndroidModelFile: ConfigPlugin<ConfigProps> = (
  c,
  { modelName, modelDir }
) => {
  return withDangerousMod(c, [
    'android',
    (config) => {
      if (!modelName) {
        console.info('No model name provided, skipping model file copy');
        return config;
      }

      const projectRoot = config.modRequest.projectRoot;
      const platformProjectRoot = config.modRequest.platformProjectRoot;

      const modelFilePath = path.resolve(
        projectRoot,
        modelDir || '',
        `${modelName}`
      );
      const destDir = path.resolve(platformProjectRoot, 'app/src/main/assets');
      const destinationPath = path.join(destDir, modelName);

      if (!fs.existsSync(modelFilePath)) {
        throw new Error(
          `Model file doesn't exist at ${modelFilePath}. Place it in your project root or configure a different path using the 'modelDir' option.`
        );
      }

      // Ensure the entire directory structure exists
      const mainDir = path.resolve(platformProjectRoot, 'app/src/main');
      if (!fs.existsSync(mainDir)) {
        fs.mkdirSync(mainDir, { recursive: true });
      }

      if (!fs.existsSync(destDir)) {
        fs.mkdirSync(destDir, { recursive: true });
      }

      // Copy the file to Android assets directory
      fs.copyFileSync(modelFilePath, destinationPath);

      console.log(
        `Copied model file from ${modelFilePath} to ${destinationPath}`
      );

      return config;
    },
  ]);
};
