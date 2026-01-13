/**
 * Configuration properties for the Expo plugin.
 * Contains settings that control the plugin's behavior.
 */
export type ConfigProps = {
  /**
   * Name of the model to be bundled (optional).
   * If not specified, no model will be bundled at build time.
   * Use launchWithRemoteModel() to download models at runtime instead.
   *
   * @example 'Pikachu' or 'Megatron'
   */
  modelName?: string;
  /**
   * Optional directory path where model files are located, relative to project root.
   * If not specified, defaults to the project root directory.
   *
   * @example 'assets/models' - looks for models in <project-root>/assets/models/<modelName>
   * @default undefined (uses project root)
   */
  modelDir?: string;
};
