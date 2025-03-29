#include "NitroInspireFace.hpp"
#include "inspireface.h"
#include <sys/stat.h>
#include <stdexcept>
#include <NitroModules/NitroLogger.hpp>
#include <cstring>

namespace margelo::nitro::nitroinspireface
{
  // // Helper function to check if a path exists
  // bool pathExists(const std::string &path)
  // {
  //   struct stat buffer;
  //   return (stat(path.c_str(), &buffer) == 0);
  // }

  double NitroInspireFace::multiply(double a, double b)
  {
    return a * b;
  }

  std::string NitroInspireFace::getVersion()
  {
    HFInspireFaceVersion version;
    HFQueryInspireFaceVersion(&version);
    return std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
    // return "1.0.0";
  }

  void NitroInspireFace::launch(const std::string &path)
  {
    // Initialize the InspireFace SDK with the path to the resource files
    HResult result = HFLaunchInspireFace(path.c_str());

    // Check specific error codes
    if (result == HERR_ARCHIVE_LOAD_MODEL_FAILURE)
    {
      throw std::runtime_error("Failed to load InspireFace model files. Please check if model files exist in: " + path);
    }
    else if (result == HERR_ARCHIVE_LOAD_FAILURE)
    {
      throw std::runtime_error("Failed to load InspireFace archive. Please check if resource files exist in: " + path);
    }
    else if (result == HERR_ARCHIVE_FILE_FORMAT_ERROR)
    {
      throw std::runtime_error("Invalid InspireFace archive format in: " + path);
    }
    else if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to initialize InspireFace SDK with error code: " + std::to_string(result));
    }
  }

  void NitroInspireFace::featureHubDataEnable(const FeatureHubConfiguration &config)
  {
    // Convert the configuration to C struct
    HFFeatureHubConfiguration cConfig;
    cConfig.primaryKeyMode = static_cast<HFPKMode>(config.primaryKeyMode);
    cConfig.enablePersistence = config.enablePersistence ? 1 : 0;

    // Create a non-const char buffer for the path
    char *pathBuffer = new char[config.persistenceDbPath.length() + 1];
    std::strcpy(pathBuffer, config.persistenceDbPath.c_str());
    cConfig.persistenceDbPath = pathBuffer;

    cConfig.searchThreshold = static_cast<float>(config.searchThreshold);
    cConfig.searchMode = static_cast<HFSearchMode>(config.searchMode);

    // Enable the feature hub
    HResult result = HFFeatureHubDataEnable(cConfig);

    // Clean up allocated memory
    delete[] pathBuffer;

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to enable FeatureHub with error code: " + std::to_string(result));
    }
  }

  void NitroInspireFace::featureHubFaceSearchThresholdSetting(double threshold)
  {
    HResult result = HFFeatureHubFaceSearchThresholdSetting(static_cast<float>(threshold));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set face search threshold with error code: " + std::to_string(result));
    }
  }

  std::shared_ptr<HybridSessionSpec> NitroInspireFace::createSession(
      const SessionCustomParameter &parameter,
      DetectMode detectMode,
      double maxDetectFaceNum,
      double detectPixelLevel,
      double trackByDetectModeFPS)
  {
    // Convert the parameter to C struct
    HFSessionCustomParameter cParam;
    cParam.enable_recognition = parameter.enableRecognition ? 1 : 0;
    cParam.enable_liveness = parameter.enableLiveness ? 1 : 0;
    cParam.enable_ir_liveness = parameter.enableIrLiveness ? 1 : 0;
    cParam.enable_mask_detect = parameter.enableMaskDetect ? 1 : 0;
    cParam.enable_face_quality = parameter.enableFaceQuality ? 1 : 0;
    cParam.enable_face_attribute = parameter.enableFaceAttribute ? 1 : 0;
    cParam.enable_interaction_liveness = parameter.enableInteractionLiveness ? 1 : 0;
    cParam.enable_detect_mode_landmark = parameter.enableDetectModeLandmark ? 1 : 0;

    HFSession session = nullptr;
    HResult result = HFCreateInspireFaceSession(
        cParam,
        static_cast<HFDetectMode>(detectMode),
        static_cast<HInt32>(maxDetectFaceNum),
        static_cast<HInt32>(detectPixelLevel),
        static_cast<HInt32>(trackByDetectModeFPS),
        &session);

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to create session with error code: " + std::to_string(result));
    }

    return std::make_shared<NitroSession>(session);
  }
} // namespace margelo::nitro::nitroinspireface
