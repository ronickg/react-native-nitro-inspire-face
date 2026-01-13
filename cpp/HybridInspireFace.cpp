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
  HybridInspireFace::HybridInspireFace() : HybridObject(TAG)
  {
    auto utilsObject = HybridObjectRegistry::createHybridObject("AssetManager");
    if (!utilsObject)
    {
      Logger::log(LogLevel::Error, TAG, "AssetManager not found in HybridObjectRegistry!");
      throw std::runtime_error("Failed to create AssetManager");
    }

    assetManager = std::dynamic_pointer_cast<HybridAssetManagerSpec>(utilsObject);
    if (!assetManager)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to cast AssetManager to HybridAssetManagerSpec!");
      throw std::runtime_error("Failed to cast AssetManager");
    }
  }

  std::string HybridInspireFace::getVersion()
  {
    HFInspireFaceVersion version;
    HFQueryInspireFaceVersion(&version);
    return std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
    // return "1.0.0";
  }

  void HybridInspireFace::launch(const std::string &path)
  {
    try
    {
      // Auto-detect if path is absolute (already in filesystem) or asset name (needs copying)
      // Absolute paths start with '/' on iOS/Android
      const bool isAbsolutePath = !path.empty() && path[0] == '/';
      std::string launchPath;

      if (isAbsolutePath)
      {
        // Direct filesystem path - no copying needed (e.g., downloaded models)
        launchPath = path;
      }
      else
      {
        // Asset name - copy from bundle first (e.g., bundled models)
        Logger::log(LogLevel::Info, TAG, "Launching InspireFace from bundled asset: %s", path.c_str());

        const std::string filesDirectory = assetManager->getFilesDirectory();
        const std::string destPath = filesDirectory + "/" + path;

        // Copy asset file
        if (!assetManager->copyAssetToFile(path, destPath))
        {
          Logger::log(LogLevel::Error, TAG, "Failed to copy asset file to '%s'", destPath.c_str());
          throw std::runtime_error("Failed to copy asset file");
        }

        Logger::log(LogLevel::Info, TAG, "Asset copied to: %s", destPath.c_str());
        launchPath = destPath;
      }

      Logger::log(LogLevel::Info, TAG, "Launching InspireFace from path: %s", launchPath.c_str());
      const HResult result = HFLaunchInspireFace(launchPath.c_str());
      if (result != HSUCCEED)
      {
        Logger::log(LogLevel::Error, TAG, "Failed to launch InspireFace SDK with error code: %ld", result);
        throw std::runtime_error("Failed to launch InspireFace SDK");
      }
    }
    catch (const std::exception &e)
    {
      Logger::log(LogLevel::Error, TAG, "Unexpected error during launch: %s", e.what());
      throw;
    }
  }

  void HybridInspireFace::reload(const std::string &path)
  {
    HResult result = HFReloadInspireFace(path.c_str());
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to reload InspireFace with error code: %ld", result);
      throw std::runtime_error("Failed to reload InspireFace");
    }
  }

  void HybridInspireFace::terminate()
  {
    HResult result = HFTerminateInspireFace();
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to terminate InspireFace with error code: %ld", result);
      throw std::runtime_error("Failed to terminate InspireFace");
    }
  }

  void HybridInspireFace::featureHubDataEnable(const FeatureHubConfiguration &config)
  {
    const std::string databasesDirectory = assetManager->getDatabasesDirectory();
    const std::string destPath = databasesDirectory + "/" + config.persistenceDbPath;

    HFFeatureHubConfiguration hfConfig;
    hfConfig.searchMode = static_cast<HFSearchMode>(config.searchMode);
    hfConfig.enablePersistence = config.enablePersistence ? 1 : 0;
    // Create a non-const buffer for the path
    char *pathBuffer = new char[destPath.length() + 1];
    std::strcpy(pathBuffer, destPath.c_str());
    hfConfig.persistenceDbPath = pathBuffer;
    hfConfig.searchThreshold = static_cast<float>(config.searchThreshold);
    hfConfig.primaryKeyMode = static_cast<HFPKMode>(config.primaryKeyMode);

    HResult result = HFFeatureHubDataEnable(hfConfig);

    // Clean up the path buffer
    delete[] pathBuffer;

    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to enable feature hub data with error code: %ld", result);
      throw std::runtime_error("Failed to enable feature hub data");
    }
  }

  void HybridInspireFace::featureHubFaceSearchThresholdSetting(double threshold)
  {
    HResult result = HFFeatureHubFaceSearchThresholdSetting(static_cast<float>(threshold));
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to set feature hub face search threshold with error code: %ld", result);
      throw std::runtime_error("Failed to set feature hub face search threshold");
    }
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

  std::vector<Point2f> HybridInspireFace::getFaceDenseLandmarkFromFaceToken(const std::shared_ptr<ArrayBuffer> &token, std::optional<double> num)
  {
    // Get the number of landmarks from the HybridInspireFace API if not provided
    int32_t numLandmarks = 0;
    if (num.has_value())
    {
      numLandmarks = static_cast<int32_t>(num.value());
    }
    else
    {
      HResult result = HFGetNumOfFaceDenseLandmark(&numLandmarks);
      if (result != HSUCCEED || numLandmarks <= 0)
      {
        throw std::runtime_error("Failed to get number of face landmarks with error code: " + std::to_string(result));
      }
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
    HResult result = HFGetFaceDenseLandmarkFromFaceToken(faceToken, landmarks.get(), numLandmarks);
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

  std::vector<Point2f> HybridInspireFace::getFaceFiveKeyPointsFromFaceToken(const std::shared_ptr<ArrayBuffer> &token, std::optional<double> num)
  {
    // Default to 5 key points if not specified
    int32_t numKeyPoints = num.has_value() ? static_cast<int32_t>(num.value()) : 5;

    // Create the HFFaceBasicToken structure from our token
    HFFaceBasicToken faceToken;
    faceToken.size = static_cast<HInt32>(token->size());

    // Get the raw data from the ArrayBuffer
    if (!token || token->size() == 0)
    {
      throw std::runtime_error("Invalid face token data");
    }
    faceToken.data = reinterpret_cast<void *>(token->data());

    // Allocate memory for key points
    auto keyPoints = std::make_unique<HPoint2f[]>(numKeyPoints);

    // Get the key points from the token
    HResult result = HFGetFaceFiveKeyPointsFromFaceToken(faceToken, keyPoints.get(), numKeyPoints);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get face five key points with error code: " + std::to_string(result));
    }

    // Convert to Point2f vector
    std::vector<Point2f> keyPointsVector;
    keyPointsVector.reserve(numKeyPoints);

    for (int i = 0; i < numKeyPoints; i++)
    {
      keyPointsVector.emplace_back(
          static_cast<double>(keyPoints[i].x),
          static_cast<double>(keyPoints[i].y));
    }

    return keyPointsVector;
  }

  double HybridInspireFace::featureHubFaceInsert(const FaceFeatureIdentity &feature)
  {
    if (!feature.feature || feature.feature->size() == 0)
    {
      throw std::runtime_error("Invalid feature data");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size - feature.feature->size() is in bytes
    if (feature.feature->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create feature struct
    HFFaceFeature hfFeature;
    hfFeature.size = expectedLength; // HFFaceFeature.size is in number of floats
    hfFeature.data = reinterpret_cast<float *>(feature.feature->data());

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
    if (!feature.feature || feature.feature->size() == 0)
    {
      throw std::runtime_error("Invalid feature data");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size - feature.feature->size() is in bytes
    if (feature.feature->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create feature struct
    HFFaceFeature hfFeature;
    hfFeature.size = expectedLength; // HFFaceFeature.size is in number of floats
    hfFeature.data = reinterpret_cast<float *>(feature.feature->data());

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

  std::optional<FaceFeatureIdentity> HybridInspireFace::featureHubFaceSearch(const std::shared_ptr<ArrayBuffer> &feature)
  {
    if (!feature || feature->size() == 0)
    {
      throw std::runtime_error("Invalid feature data");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size - feature->size() is in bytes
    if (feature->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create feature struct
    HFFaceFeature hfFeature;
    hfFeature.size = expectedLength; // HFFaceFeature.size is in number of floats
    hfFeature.data = reinterpret_cast<float *>(feature->data());

    // Search for face
    HFloat confidence;
    HFFaceFeatureIdentity identity;
    HResult result = HFFeatureHubFaceSearch(hfFeature, &confidence, &identity);

    if (result != HSUCCEED)
    {
      return std::nullopt;
    }

    // Create ArrayBuffer for feature data - multiply by sizeof(float) since we need bytes
    auto featureBuffer = ArrayBuffer::copy(
        reinterpret_cast<uint8_t *>(identity.feature->data),
        identity.feature->size * sizeof(float));

    return FaceFeatureIdentity(
        static_cast<double>(identity.id),
        featureBuffer,
        static_cast<double>(confidence));
  }

  std::optional<FaceFeatureIdentity> HybridInspireFace::featureHubGetFaceIdentity(double id)
  {
    HFFaceFeatureIdentity identity = {};
    HResult result = HFFeatureHubGetFaceIdentity(static_cast<HFaceId>(id), &identity);
    if (result != HSUCCEED || !identity.feature)
    {
      // If the ID doesn't exist or feature is null, return nullopt instead of throwing an exception
      return std::nullopt;
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size - identity.feature->size is already in number of floats
    if (identity.feature->size != expectedLength)
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create ArrayBuffer from feature data - multiply by sizeof(float) to get byte size
    HFFaceFeature *feature = identity.feature;
    auto featureBuffer = ArrayBuffer::copy(
        reinterpret_cast<uint8_t *>(feature->data),
        feature->size * sizeof(float));

    // Return the FaceFeatureIdentity with no confidence (since this is just a retrieval)
    return FaceFeatureIdentity(
        static_cast<double>(identity.id),
        featureBuffer,
        std::nullopt);
  }

  std::vector<SearchTopKResult> HybridInspireFace::featureHubFaceSearchTopK(const std::shared_ptr<ArrayBuffer> &feature, double topK)
  {
    if (!feature || feature->size() == 0)
    {
      throw std::runtime_error("Invalid feature data");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size - feature->size() is in bytes
    if (feature->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create feature struct
    HFFaceFeature hfFeature;
    hfFeature.size = expectedLength; // HFFaceFeature.size is in number of floats
    hfFeature.data = reinterpret_cast<float *>(feature->data());

    // Search for faces
    HFSearchTopKResults results;
    HResult result = HFFeatureHubFaceSearchTopK(hfFeature, static_cast<HInt32>(topK), &results);

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to search top K faces");
    }

    // Convert results to vector
    std::vector<SearchTopKResult> searchResults;
    searchResults.reserve(results.size);
    for (int i = 0; i < results.size; i++)
    {
      searchResults.emplace_back(
          static_cast<double>(results.confidence[i]),
          static_cast<double>(results.ids[i]));
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

  double HybridInspireFace::getFaceDenseLandmarkLength()
  {
    HInt32 length = 0;
    HResult result = HFGetNumOfFaceDenseLandmark(&length);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get face dense landmark length with error code: " + std::to_string(result));
    }
    return static_cast<double>(length);
  }

  double HybridInspireFace::getFaceBasicTokenLength()
  {
    HInt32 length = 0;
    HResult result = HFGetFaceBasicTokenSize(&length);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get face basic token length with error code: " + std::to_string(result));
    }
    return static_cast<double>(length);
  }

  double HybridInspireFace::getRecommendedCosineThreshold()
  {
    HFloat threshold = 0;
    HResult result = HFGetRecommendedCosineThreshold(&threshold);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get recommended cosine threshold with error code: " + std::to_string(result));
    }
    return static_cast<double>(threshold);
  }

  double HybridInspireFace::cosineSimilarityConvertToPercentage(double similarity)
  {
    HFloat percentage = 0;
    HResult result = HFCosineSimilarityConvertToPercentage(static_cast<HFloat>(similarity), &percentage);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to convert cosine similarity to percentage with error code: " + std::to_string(result));
    }
    return static_cast<double>(percentage);
  }

  void HybridInspireFace::updateCosineSimilarityConverter(const SimilarityConverterConfig &config)
  {
    // Convert our config to the C API config
    ::HFSimilarityConverterConfig cConfig;
    cConfig.threshold = config.threshold;
    cConfig.middleScore = config.middleScore;
    cConfig.steepness = config.steepness;
    cConfig.outputMin = config.outputMin;
    cConfig.outputMax = config.outputMax;

    HResult result = HFUpdateCosineSimilarityConverter(cConfig);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to update cosine similarity converter with error code: " + std::to_string(result));
    }
  }

  SimilarityConverterConfig HybridInspireFace::getCosineSimilarityConverter()
  {
    ::HFSimilarityConverterConfig cConfig;
    HResult result = HFGetCosineSimilarityConverter(&cConfig);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get cosine similarity converter with error code: " + std::to_string(result));
    }

    // Convert C API config to our config
    SimilarityConverterConfig config;
    config.threshold = cConfig.threshold;
    config.middleScore = cConfig.middleScore;
    config.steepness = cConfig.steepness;
    config.outputMin = cConfig.outputMin;
    config.outputMax = cConfig.outputMax;
    return config;
  }

  double HybridInspireFace::featureHubGetFaceCount()
  {
    HInt32 count = 0;
    HResult result = HFFeatureHubGetFaceCount(&count);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get face count with error code: " + std::to_string(result));
    }
    return static_cast<double>(count);
  }

  std::vector<double> HybridInspireFace::featureHubGetExistingIds()
  {
    HFFeatureHubExistingIds ids = {};
    HResult result = HFFeatureHubGetExistingIds(&ids);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get existing ids with error code: " + std::to_string(result));
    }

    std::vector<double> idVector;
    if (ids.size > 0 && ids.ids != nullptr)
    {
      idVector.reserve(ids.size);
      for (int i = 0; i < ids.size; i++)
      {
        idVector.push_back(static_cast<double>(ids.ids[i]));
      }
    }

    return idVector;
  }

  void HybridInspireFace::featureHubDataDisable()
  {
    HResult result = HFFeatureHubDataDisable();
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to disable feature hub with error code: " + std::to_string(result));
    }
  }

  double HybridInspireFace::faceComparison(const std::shared_ptr<ArrayBuffer> &feature1, const std::shared_ptr<ArrayBuffer> &feature2)
  {
    if (!feature1 || !feature2 || feature1->size() == 0 || feature2->size() == 0)
    {
      throw std::runtime_error("Invalid feature data");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size
    if (feature1->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }
    if (feature2->size() != expectedLength * sizeof(float))
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create feature structs
    HFFaceFeature hfFeature1;
    hfFeature1.size = expectedLength;
    hfFeature1.data = reinterpret_cast<float *>(feature1->data());

    HFFaceFeature hfFeature2;
    hfFeature2.size = expectedLength;
    hfFeature2.data = reinterpret_cast<float *>(feature2->data());

    // Compare features
    HFloat result;
    HResult status = HFFaceComparison(hfFeature1, hfFeature2, &result);

    if (status != HSUCCEED)
    {
      throw std::runtime_error("Failed to compare faces");
    }

    return static_cast<double>(result);
  }

  void HybridInspireFace::setExpansiveHardwareRockchipDmaHeapPath(const std::string &path)
  {
    HResult result = HFSetExpansiveHardwareRockchipDmaHeapPath(path.c_str());
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to set rockchip dma heap path with error code: %ld", result);
      throw std::runtime_error("Failed to set rockchip dma heap path");
    }
  }

  std::string HybridInspireFace::queryExpansiveHardwareRockchipDmaHeapPath()
  {
    char path[256];
    HResult result = HFQueryExpansiveHardwareRockchipDmaHeapPath(path);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to query rockchip dma heap path with error code: %ld", result);
      throw std::runtime_error("Failed to query rockchip dma heap path");
    }
    return std::string(path);
  }

  void HybridInspireFace::setAppleCoreMLInferenceMode(AppleCoreMLInferenceMode mode)
  {
    HResult result = HFSetAppleCoreMLInferenceMode(static_cast<HFAppleCoreMLInferenceMode>(mode));
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to set Apple CoreML inference mode with error code: %ld", result);
      throw std::runtime_error("Failed to set Apple CoreML inference mode");
    }
  }

  void HybridInspireFace::setCudaDeviceId(double deviceId)
  {
    HResult result = HFSetCudaDeviceId(static_cast<int32_t>(deviceId));
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to set CUDA device ID with error code: %ld", result);
      throw std::runtime_error("Failed to set CUDA device ID");
    }
  }

  double HybridInspireFace::getCudaDeviceId()
  {
    int32_t deviceId;
    HResult result = HFGetCudaDeviceId(&deviceId);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to get CUDA device ID with error code: %ld", result);
      throw std::runtime_error("Failed to get CUDA device ID");
    }
    return static_cast<double>(deviceId);
  }

  void HybridInspireFace::printCudaDeviceInfo()
  {
    HResult result = HFPrintCudaDeviceInfo();
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to print CUDA device info with error code: %ld", result);
      throw std::runtime_error("Failed to print CUDA device info");
    }
  }

  double HybridInspireFace::getNumCudaDevices()
  {
    int32_t numDevices;
    HResult result = HFGetNumCudaDevices(&numDevices);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to get number of CUDA devices with error code: %ld", result);
      throw std::runtime_error("Failed to get number of CUDA devices");
    }
    return static_cast<double>(numDevices);
  }

  bool HybridInspireFace::checkCudaDeviceSupport()
  {
    int32_t isSupported = 0;
    HResult result = HFCheckCudaDeviceSupport(&isSupported);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, TAG, "Failed to check CUDA device support with error code: %ld", result);
      throw std::runtime_error("Failed to check CUDA device support");
    }
    return isSupported != 0;
  }

  std::shared_ptr<ArrayBuffer> HybridInspireFace::fromBase64(const std::string &base64)
  {
    std::vector<unsigned char> decoded = base64_decode(base64);
    if (decoded.empty())
    {
      throw std::runtime_error("Failed to decode base64 string");
    }
    return ArrayBuffer::copy(decoded.data(), decoded.size());
  }

  std::string HybridInspireFace::toBase64(const std::shared_ptr<ArrayBuffer> &buffer)
  {
    if (!buffer || buffer->size() == 0)
    {
      throw std::runtime_error("Invalid buffer");
    }
    return base64_encode(reinterpret_cast<const unsigned char *>(buffer->data()), buffer->size());
  }

  // Add these helper functions to FSDKApi class
  std::string HybridInspireFace::base64_encode(const unsigned char *data, size_t len)
  {
    static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string ret;
    ret.reserve(((len + 2) / 3) * 4);

    for (size_t i = 0; i < len; i += 3)
    {
      unsigned char b3[3] = {0};
      for (size_t j = 0; j < 3; j++)
      {
        if (i + j < len)
          b3[j] = data[i + j];
      }

      ret.push_back(base64_chars[(b3[0] & 0xfc) >> 2]);
      ret.push_back(base64_chars[((b3[0] & 0x03) << 4) + ((b3[1] & 0xf0) >> 4)]);
      ret.push_back(i + 1 < len ? base64_chars[((b3[1] & 0x0f) << 2) + ((b3[2] & 0xc0) >> 6)] : '=');
      ret.push_back(i + 2 < len ? base64_chars[b3[2] & 0x3f] : '=');
    }

    return ret;
  }

  std::vector<unsigned char> HybridInspireFace::base64_decode(const std::string &encoded)
  {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::vector<unsigned char> ret;
    int val = 0, valb = -8;

    for (unsigned char c : encoded)
    {
      if (c == '=')
        break;
      if (base64_chars.find(c) == std::string::npos)
        continue;

      val = (val << 6) + base64_chars.find(c);
      valb += 6;

      if (valb >= 0)
      {
        ret.push_back(char((val >> valb) & 0xFF));
        valb -= 8;
      }
    }

    return ret;
  }

} // namespace margelo::nitro::nitroinspireface
