#include "HybridInspireFace.hpp"
#include "inspireface.h"
#include <sys/stat.h>
#include <stdexcept>
#include <NitroModules/NitroLogger.hpp>
#include <NitroModules/HybridObjectRegistry.hpp>
#include <cstring>
#include <memory>
#include <vector>
#include <optional>

namespace margelo::nitro::nitroinspireface
{
  std::string HybridInspireFace::getVersion()
  {
    HFInspireFaceVersion version;
    HFQueryInspireFaceVersion(&version);
    return std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
    // return "1.0.0";
  }

  bool HybridInspireFace::launch(const std::string &path)
  {
    // Get utils object and validate
    auto utilsObject = HybridObjectRegistry::createHybridObject("AssetManager");
    if (!utilsObject)
    {
      Logger::log(LogLevel::Error, TAG, "AssetManager not found in HybridObjectRegistry!");
      return false;
    }

    auto utils = std::dynamic_pointer_cast<HybridAssetManagerSpec>(utilsObject);
    if (!utils)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to cast AssetManager to HybridAssetManagerSpec!");
      return false;
    }

    try
    {
      // Get base directory and construct destination path
      const std::string baseDirectory = utils->getBaseDirectory();
      const std::string destPath = baseDirectory + "/" + path;

      // Copy asset file
      if (!utils->copyAssetToFile(path, destPath))
      {
        Logger::log(LogLevel::Error, TAG, "Failed to copy asset file to '%s'", destPath.c_str());
        return false;
      }
      // Log success and launch
      Logger::log(LogLevel::Info, TAG, "Launching HybridInspireFace from: %s", destPath.c_str());

      const HResult result = HFLaunchInspireFace(destPath.c_str());
      if (result != HSUCCEED)
      {
        Logger::log(LogLevel::Error, TAG, "Failed to launch HybridInspireFace SDK with error code: %ld", result);
        return false;
      }

      return true;
    }
    catch (const std::exception &e)
    {
      Logger::log(LogLevel::Error, TAG, "Unexpected error during launch: %s", e.what());
      return false;
    }
  }

  bool HybridInspireFace::featureHubDataEnable(const FeatureHubConfiguration &config)
  {
    HFFeatureHubConfiguration hfConfig;
    hfConfig.searchMode = static_cast<HFSearchMode>(config.searchMode);
    hfConfig.enablePersistence = config.enablePersistence ? 1 : 0;
    // Create a non-const buffer for the path
    char *pathBuffer = new char[config.persistenceDbPath.length() + 1];
    std::strcpy(pathBuffer, config.persistenceDbPath.c_str());
    hfConfig.persistenceDbPath = pathBuffer;
    hfConfig.searchThreshold = static_cast<float>(config.searchThreshold);
    hfConfig.primaryKeyMode = static_cast<HFPKMode>(config.primaryKeyMode);

    HResult result = HFFeatureHubDataEnable(hfConfig);

    // Clean up the path buffer
    delete[] pathBuffer;

    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to enable feature hub data with error code: %ld", result);
      return false;
    }
    return true;
  }

  bool HybridInspireFace::featureHubFaceSearchThresholdSetting(double threshold)
  {
    HResult result = HFFeatureHubFaceSearchThresholdSetting(static_cast<float>(threshold));
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to set feature hub face search threshold with error code: %ld", result);
      return false;
    }
    return true;
  }

  std::shared_ptr<HybridSessionSpec> HybridInspireFace::createSession(
      const SessionCustomParameter &parameter,
      DetectMode detectMode,
      double maxDetectFaceNum,
      double detectPixelLevel,
      double trackByDetectModeFPS)
  {
    HFSessionCustomParameter hfParam;
    hfParam.enable_recognition = parameter.enableRecognition ? 1 : 0;
    hfParam.enable_liveness = parameter.enableLiveness ? 1 : 0;
    hfParam.enable_ir_liveness = parameter.enableIrLiveness ? 1 : 0;
    hfParam.enable_mask_detect = parameter.enableMaskDetect ? 1 : 0;
    hfParam.enable_face_quality = parameter.enableFaceQuality ? 1 : 0;
    hfParam.enable_face_attribute = parameter.enableFaceAttribute ? 1 : 0;
    hfParam.enable_interaction_liveness = parameter.enableInteractionLiveness ? 1 : 0;
    hfParam.enable_detect_mode_landmark = parameter.enableDetectModeLandmark ? 1 : 0;

    HFSession session = nullptr;
    HResult result = HFCreateInspireFaceSession(
        hfParam,
        static_cast<HFDetectMode>(detectMode),
        static_cast<HInt32>(maxDetectFaceNum),
        static_cast<HInt32>(detectPixelLevel),
        static_cast<HInt32>(trackByDetectModeFPS),
        &session);

    if (result != HSUCCEED || session == nullptr)
    {
      throw std::runtime_error("Failed to create session with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridSession>(session);
  }

  std::shared_ptr<HybridImageBitmapSpec> HybridInspireFace::createImageBitmapFromFilePath(double channels, const std::string &filePath)
  {
    HFImageBitmap bitmap = nullptr;
    HResult result = HFCreateImageBitmapFromFilePath(filePath.c_str(), static_cast<HInt32>(channels), &bitmap);
    if (result != HSUCCEED || bitmap == nullptr)
    {
      throw std::runtime_error("Failed to create image bitmap from file with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(bitmap);
  }

  std::shared_ptr<HybridImageBitmapSpec> HybridInspireFace::createImageBitmapFromBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height, double channels)
  {
    // Create bitmap data structure
    HFImageBitmapData bitmapData{};
    bitmapData.data = reinterpret_cast<uint8_t *>(buffer->data());
    bitmapData.width = static_cast<HInt32>(width);
    bitmapData.height = static_cast<HInt32>(height);
    bitmapData.channels = static_cast<HInt32>(channels);

    // Create bitmap from data
    HFImageBitmap bitmap = nullptr;
    HResult result = HFCreateImageBitmap(&bitmapData, &bitmap);
    if (result != HSUCCEED || bitmap == nullptr)
    {
      throw std::runtime_error("Failed to create image bitmap from buffer with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(bitmap);
  }

  std::shared_ptr<HybridImageStreamSpec> HybridInspireFace::createImageStreamFromBitmap(const std::shared_ptr<HybridImageBitmapSpec> &bitmap, CameraRotation rotation)
  {
    if (!bitmap)
    {
      throw std::runtime_error("Invalid bitmap");
    }

    auto nitroBitmap = std::dynamic_pointer_cast<HybridImageBitmap>(bitmap);
    if (!nitroBitmap)
    {
      throw std::runtime_error("Failed to cast to HybridImageBitmap");
    }

    // Create stream from bitmap
    HFImageStream stream = nullptr;
    HResult result = HFCreateImageStreamFromImageBitmap(
        nitroBitmap->getNativeHandle(),
        static_cast<HFRotation>(rotation),
        &stream);

    if (result != HSUCCEED || stream == nullptr)
    {
      throw std::runtime_error("Failed to create image stream from bitmap with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageStream>(stream);
  }

  std::vector<Point2f> HybridInspireFace::getFaceDenseLandmarkFromFaceToken(const std::shared_ptr<ArrayBuffer> &token)
  {
    // Get the number of landmarks from the HybridInspireFace API
    int32_t numLandmarks = 0;
    HResult result = HFGetNumOfFaceDenseLandmark(&numLandmarks);
    if (result != HSUCCEED || numLandmarks <= 0)
    {
      throw std::runtime_error("Failed to get number of face landmarks with error code: " + std::to_string(result));
    }

    // Create the HFFaceBasicToken structure from our token
    HFFaceBasicToken faceToken;
    faceToken.size = static_cast<HInt32>(token->size());

    // Get the raw data from the ArrayBuffer
    if (!token || token->size() == 0)
    {
      throw std::runtime_error("Invalid face token data");
    }
    faceToken.data = reinterpret_cast<void *>(token->data());

    // Allocate memory for landmarks
    auto landmarks = std::make_unique<HPoint2f[]>(numLandmarks);

    // Get the landmarks from the token
    result = HFGetFaceDenseLandmarkFromFaceToken(faceToken, landmarks.get(), numLandmarks);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get face dense landmarks with error code: " + std::to_string(result));
    }

    // Convert to Point2f vector
    std::vector<Point2f> landmarkPoints;
    landmarkPoints.reserve(numLandmarks);

    for (int i = 0; i < numLandmarks; i++)
    {
      landmarkPoints.emplace_back(
          static_cast<double>(landmarks[i].x),
          static_cast<double>(landmarks[i].y));
    }

    return landmarkPoints;
  }

  double HybridInspireFace::featureHubFaceInsert(const FaceFeatureIdentity &feature)
  {
    // Convert std::vector<double> to HFFaceFeature
    HFFaceFeature hfFeature;
    hfFeature.size = static_cast<HInt32>(feature.feature.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> featureFloat(feature.feature.begin(), feature.feature.end());
    hfFeature.data = reinterpret_cast<HPFloat>(featureFloat.data());

    // Create feature identity struct
    HFFaceFeatureIdentity identity;
    identity.id = static_cast<HFaceId>(feature.id);
    identity.feature = &hfFeature;

    // Insert the feature
    HFaceId allocId;
    HResult result = HFFeatureHubInsertFeature(identity, &allocId);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to insert feature with error code: " + std::to_string(result));
    }
    return allocId;
  }

  bool HybridInspireFace::featureHubFaceUpdate(const FaceFeatureIdentity &feature)
  {
    // Convert std::vector<double> to HFFaceFeature
    HFFaceFeature hfFeature;
    hfFeature.size = static_cast<HInt32>(feature.feature.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> featureFloat(feature.feature.begin(), feature.feature.end());
    hfFeature.data = reinterpret_cast<HPFloat>(featureFloat.data());

    // Create feature identity struct
    HFFaceFeatureIdentity identity;
    identity.id = static_cast<HFaceId>(feature.id);
    identity.feature = &hfFeature;

    // Update the feature
    HResult result = HFFeatureHubFaceUpdate(identity);
    return result == HSUCCEED;
  }

  bool HybridInspireFace::featureHubFaceRemove(double id)
  {
    HResult result = HFFeatureHubFaceRemove(static_cast<HFaceId>(id));
    return result == HSUCCEED;
  }

  std::optional<FaceFeatureIdentity> HybridInspireFace::featureHubFaceSearch(const std::vector<double> &feature)
  {
    // Convert vector<double> to HFFaceFeature
    HFFaceFeature hfFeature;
    hfFeature.size = static_cast<HInt32>(feature.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> featureFloat(feature.begin(), feature.end());
    hfFeature.data = reinterpret_cast<HPFloat>(featureFloat.data());

    // Create variables for search results
    HFloat confidence = 0;
    HFFaceFeatureIdentity mostSimilar = {};

    // Search for the face
    HResult result = HFFeatureHubFaceSearch(hfFeature, &confidence, &mostSimilar);
    if (result != HSUCCEED)
    {
      // If the ID doesn't exist, return nullopt instead of throwing an exception
      return std::nullopt;
    }

    // Convert HFFaceFeature to std::vector<double>
    std::vector<double> resultFeature;
    resultFeature.reserve(mostSimilar.feature->size);
    for (int i = 0; i < mostSimilar.feature->size; i++)
    {
      resultFeature.push_back(static_cast<double>(((float *)mostSimilar.feature->data)[i]));
    }

    // Return the FaceFeatureIdentity with confidence
    return FaceFeatureIdentity(
        static_cast<double>(mostSimilar.id),
        resultFeature,
        std::make_optional(static_cast<double>(confidence)));
  }

  std::optional<FaceFeatureIdentity> HybridInspireFace::featureHubGetFaceIdentity(double id)
  {
    HFFaceFeatureIdentity identity = {};
    HResult result = HFFeatureHubGetFaceIdentity(static_cast<HFaceId>(id), &identity);
    if (result != HSUCCEED)
    {
      // If the ID doesn't exist, return nullopt instead of throwing an exception
      return std::nullopt;
    }

    // Convert HFFaceFeature to std::vector<double>
    std::vector<double> resultFeature;
    resultFeature.reserve(identity.feature->size);
    for (int i = 0; i < identity.feature->size; i++)
    {
      resultFeature.push_back(static_cast<double>(((float *)identity.feature->data)[i]));
    }

    // Return the FaceFeatureIdentity with no confidence (since this is just a retrieval)
    return FaceFeatureIdentity(
        static_cast<double>(identity.id),
        resultFeature,
        std::nullopt);
  }

  std::vector<SearchTopKResult> HybridInspireFace::featureHubFaceSearchTopK(const std::vector<double> &feature, double topK)
  {
    // Convert vector<double> to HFFaceFeature
    HFFaceFeature hfFeature;
    hfFeature.size = static_cast<HInt32>(feature.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> featureFloat(feature.begin(), feature.end());
    hfFeature.data = reinterpret_cast<HPFloat>(featureFloat.data());

    // Create search results struct
    HFSearchTopKResults results = {};
    HResult result = HFFeatureHubFaceSearchTopK(hfFeature, static_cast<HInt32>(topK), &results);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to search top-k faces with error code: " + std::to_string(result));
    }

    // Convert results to vector of SearchTopKResult
    std::vector<SearchTopKResult> searchResults;
    searchResults.reserve(results.size);
    for (int i = 0; i < results.size; i++)
    {
      searchResults.push_back(SearchTopKResult(
          static_cast<double>(results.confidence[i]),
          static_cast<double>(results.ids[i])));
    }

    return searchResults;
  }

  double HybridInspireFace::getFeatureLength()
  {
    HInt32 length = 0;
    HResult result = HFGetFeatureLength(&length);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length with error code: " + std::to_string(result));
    }
    return static_cast<double>(length);
  }

  double HybridInspireFace::faceComparison(const std::vector<double> &feature1, const std::vector<double> &feature2)
  {
    // Convert std::vector<double> to HFFaceFeature for the first feature
    HFFaceFeature hfFeature1 = {};
    hfFeature1.size = static_cast<HInt32>(feature1.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> feature1Float(feature1.begin(), feature1.end());
    hfFeature1.data = reinterpret_cast<HPFloat>(feature1Float.data());

    // Convert std::vector<double> to HFFaceFeature for the second feature
    HFFaceFeature hfFeature2 = {};
    hfFeature2.size = static_cast<HInt32>(feature2.size());

    // Create a temporary buffer for the HPFloat data
    std::vector<float> feature2Float(feature2.begin(), feature2.end());
    hfFeature2.data = reinterpret_cast<HPFloat>(feature2Float.data());

    // Create variable to store comparison result
    HFloat result_value = 0.0f;

    // Perform face comparison
    HResult result = HFFaceComparison(hfFeature1, hfFeature2, &result_value);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to compare faces with error code: " + std::to_string(result));
    }

    // Return the comparison result
    return static_cast<double>(result_value);
  }

} // namespace margelo::nitro::nitroinspireface
