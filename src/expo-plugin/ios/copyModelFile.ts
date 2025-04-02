import type { ConfigPlugin, XcodeProject } from '@expo/config-plugins';
import { IOSConfig, withXcodeProject } from '@expo/config-plugins';
import fs from 'fs';
import path from 'path';
import type { ConfigProps } from '../@types';

export const withIosModelFile: ConfigPlugin<ConfigProps> = (
  c,
  { modelName }
) => {
  return withXcodeProject(c, (config) => {
    config.modResults = setModelFile({
      projectRoot: config.modRequest.projectRoot,
      project: config.modResults,
      modelName,
    });
    return config;
  });
};

export function setModelFile({
  projectRoot,
  project,
  modelName,
}: {
  project: XcodeProject;
  projectRoot: string;
  modelName: string;
}): XcodeProject {
  const modelFilePath = path.resolve(projectRoot, `${modelName}`);

  if (!fs.existsSync(modelFilePath)) {
    throw new Error(
      `Model file doesn't exist at ${modelFilePath}. Place it in your project root or configure a different path.`
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
