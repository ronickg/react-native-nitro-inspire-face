import type { ConfigPlugin, XcodeProject } from '@expo/config-plugins';
import { IOSConfig, withXcodeProject } from '@expo/config-plugins';
import fs from 'fs';
import path from 'path';
import type { ConfigProps } from '../@types';

export const withIosModelFile: ConfigPlugin<ConfigProps> = (
  c,
  { modelName, modelDir }
) => {
  return withXcodeProject(c, (config) => {
    config.modResults = setModelFile({
      projectRoot: config.modRequest.projectRoot,
      project: config.modResults,
      modelName,
      modelDir,
    });
    return config;
  });
};

export function setModelFile({
  projectRoot,
  project,
  modelName,
  modelDir,
}: {
  project: XcodeProject;
  projectRoot: string;
  modelName?: string;
  modelDir?: string;
}): XcodeProject {
  if (!modelName) {
    console.info('No model name provided, skipping model file copy');
    return project;
  }

  const modelFilePath = path.resolve(
    projectRoot,
    modelDir || '',
    `${modelName}`
  );

  if (!fs.existsSync(modelFilePath)) {
    throw new Error(
      `Model file doesn't exist at ${modelFilePath}. Place it in your project root or configure a different path using the 'modelDir' option.`
    );
  }

  const sourceRoot = IOSConfig.Paths.getSourceRoot(projectRoot);
  const destinationPath = path.join(sourceRoot, `${modelName}`);

  // Copy the file to iOS project directory
  fs.copyFileSync(modelFilePath, destinationPath);

  console.log(`Copied model file from ${modelFilePath} to ${destinationPath}`);

  const projectName = IOSConfig.XcodeUtils.getProjectName(projectRoot);
  const modelFileXcodePath = `${projectName}/${modelName}`;

  // Add model file to Xcode project if it doesn't exist
  if (!project.hasFile(modelFileXcodePath)) {
    project = IOSConfig.XcodeUtils.addResourceFileToGroup({
      filepath: modelFileXcodePath,
      groupName: projectName,
      project,
      isBuildFile: true,
    });
    console.log(`Linked model file ${modelName} in Xcode project`);
  }

  return project;
}
