#include "HybridSession.hpp"
#include <stdexcept>
#include <string>
#include <cstring>
#include <NitroModules/ArrayBuffer.hpp>
#include <NitroModules/NitroLogger.hpp>
#include "FaceRect.hpp"
#include "FaceEulerAngle.hpp"
#include "FaceData.hpp"
#include "FaceInteractionState.hpp"
#include "FaceInteractionsAction.hpp"
#include "FaceAttributeResult.hpp"
#include "FaceEmotionResult.hpp"
#include "HybridImageStream.hpp"
#include <memory>
#include <vector>

namespace margelo::nitro::nitroinspireface
{
  HybridSession::HybridSession() : HybridObject(TAG), _session(nullptr) {}

  HybridSession::HybridSession(HFSession session) : HybridObject(TAG), _session(session) {}

  void HybridSession::cleanup()
  {
    // Exclusive lock: wait for any in-flight session call (shared holders)
    // before freeing the native session.
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session != nullptr)
    {
      HFReleaseInspireFaceSession(_session);
      _session = nullptr;
    }
  }

  HybridSession::~HybridSession()
  {
    cleanup();
  }

  void HybridSession::dispose()
  {
    cleanup();
  }

  void HybridSession::setTrackPreviewSize(double size)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetTrackPreviewSize(_session, static_cast<HInt32>(size));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track preview size with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setFaceDetectThreshold(double threshold)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetFaceDetectThreshold(_session, static_cast<HFloat>(threshold));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set face detect threshold with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setFilterMinimumFacePixelSize(double size)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetFilterMinimumFacePixelSize(_session, static_cast<HInt32>(size));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set minimum face pixel size with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setTrackModeSmoothRatio(double ratio)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetTrackModeSmoothRatio(_session, static_cast<HFloat>(ratio));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track mode smooth ratio with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setTrackModeNumSmoothCacheFrame(double num)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetTrackModeNumSmoothCacheFrame(_session, static_cast<HInt32>(num));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track mode num smooth cache frame with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setTrackModeDetectInterval(double num)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetTrackModeDetectInterval(_session, static_cast<HInt32>(num));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track mode detect interval with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setTrackLostRecoveryMode(bool enable)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetTrackLostRecoveryMode(_session, enable ? 1 : 0);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track lost recovery mode with error code: " + std::to_string(result));
    }
  }

  void HybridSession::setLightTrackConfidenceThreshold(double value)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionSetLightTrackConfidenceThreshold(_session, static_cast<HFloat>(value));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set light track confidence threshold with error code: " + std::to_string(result));
    }
  }

  void HybridSession::clearTrackingFace()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HResult result = HFSessionClearTrackingFace(_session);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to clear tracking face with error code: " + std::to_string(result));
    }
  }

  std::vector<FaceData> HybridSession::executeFaceTrack(const std::shared_ptr<HybridImageStreamSpec> &imageStream)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (!_session)
    {
      throw std::runtime_error("HybridSession is null");
    }
    if (!imageStream)
    {
      throw std::runtime_error("Image stream is null");
    }

    auto nitroImageStream = std::dynamic_pointer_cast<HybridImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Invalid image stream type");
    }

    HFMultipleFaceData results{};
    // withNativeHandle holds the stream's lock across the call, so a concurrent
    // stream.dispose() can't free the stream handle mid-track.
    HResult status = nitroImageStream->withNativeHandle([&](HFImageStream nativeStream)
                                                        { return HFExecuteFaceTrack(_session, nativeStream, &results); });
    if (status != HSUCCEED)
    {
      throw std::runtime_error("Face track failed with code: " + std::to_string(status));
    }

    Logger::log(LogLevel::Info, TAG, "Face track results: %d", results.detectedNum);

    // Process results into a vector
    std::vector<FaceData> faceDataVector;
    if (results.detectedNum > 0)
    {
      faceDataVector.reserve(results.detectedNum);

      for (int i = 0; i < results.detectedNum; ++i)
      {
        // Construct FaceRect
        FaceRect rect(
            static_cast<double>(results.rects[i].x),
            static_cast<double>(results.rects[i].y),
            static_cast<double>(results.rects[i].width),
            static_cast<double>(results.rects[i].height));

        // Extract track ID, track count, and confidence
        double trackId = static_cast<double>(results.trackIds[i]);
        double trackCount = static_cast<double>(results.trackCounts[i]);
        double detConfidence = static_cast<double>(results.detConfidence[i]);

        // Construct FaceEulerAngle
        FaceEulerAngle angles(
            static_cast<double>(results.angles.roll[i]),
            static_cast<double>(results.angles.yaw[i]),
            static_cast<double>(results.angles.pitch[i]));

        // Handle token data
        std::shared_ptr<margelo::nitro::ArrayBuffer> buffer;
        if (results.tokens[i].size > 0 && results.tokens[i].data != nullptr)
        {
          try
          {
            buffer = margelo::nitro::ArrayBuffer::copy(
                static_cast<uint8_t *>(results.tokens[i].data),
                results.tokens[i].size);
          }
          catch (const std::bad_alloc &e)
          {
            buffer = margelo::nitro::ArrayBuffer::allocate(0); // Fallback to empty buffer
          }
        }
        else
        {
          buffer = margelo::nitro::ArrayBuffer::allocate(0); // Empty buffer if no valid token
        }

        // Add FaceData to vector
        faceDataVector.emplace_back(rect, trackId, trackCount, detConfidence, angles, buffer);
      }
    }

    return faceDataVector;
  }

  std::shared_ptr<ArrayBuffer> HybridSession::extractFaceFeature(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    if (!imageStream || !faceToken)
    {
      throw std::runtime_error("Invalid input parameters");
    }

    // Cast the image stream to HybridImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<HybridImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to HybridImageStream");
    }

    // Create face token struct
    HFFaceBasicToken token = {};
    token.size = static_cast<HInt32>(faceToken->size());
    token.data = faceToken->data();

    // Initialize feature struct with zeros
    HFFaceFeature feature = {};

    // Extract face feature
    HResult result = nitroImageStream->withNativeHandle([&](HFImageStream h)
                                                       { return HFFaceFeatureExtract(_session, h, token, &feature); });

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to extract face feature");
    }

    // Check if feature data is valid
    if (feature.size <= 0 || feature.data == nullptr)
    {
      throw std::runtime_error("Invalid feature data returned");
    }

    // Get expected feature length
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    // Validate feature size
    if (feature.size != expectedLength)
    {
      throw std::runtime_error("Invalid feature size: expected " + std::to_string(expectedLength) + " floats");
    }

    // Create an ArrayBuffer from the feature data
    auto featureBuffer = ArrayBuffer::copy(
        reinterpret_cast<uint8_t *>(feature.data),
        feature.size * sizeof(float));

    return featureBuffer;
  }

  bool HybridSession::multipleFacePipelineProcess(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::vector<FaceData> &multipleFaceData, const SessionCustomParameter &parameter)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return false;
    }

    if (!imageStream)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Image stream is null");
      return false;
    }

    // Cast the image stream to HybridImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<HybridImageStream>(imageStream);
    if (!nitroImageStream)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to cast to HybridImageStream");
      return false;
    }

    // Convert SessionCustomParameter to HFSessionCustomParameter
    HFSessionCustomParameter hfParam;
    hfParam.enable_recognition = parameter.enableRecognition.value_or(false) ? 1 : 0;
    hfParam.enable_liveness = parameter.enableLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_ir_liveness = parameter.enableIrLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_mask_detect = parameter.enableMaskDetect.value_or(false) ? 1 : 0;
    hfParam.enable_face_quality = parameter.enableFaceQuality.value_or(false) ? 1 : 0;
    hfParam.enable_face_attribute = parameter.enableFaceAttribute.value_or(false) ? 1 : 0;
    hfParam.enable_interaction_liveness = parameter.enableInteractionLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_detect_mode_landmark = parameter.enableDetectModeLandmark.value_or(false) ? 1 : 0;
    hfParam.enable_face_pose = parameter.enableFacePose.value_or(false) ? 1 : 0;
    hfParam.enable_face_emotion = parameter.enableFaceEmotion.value_or(false) ? 1 : 0;

    // Convert vector<FaceData> to HFMultipleFaceData
    HFMultipleFaceData hfFaces = {};
    memset(&hfFaces, 0, sizeof(HFMultipleFaceData));

    // Set the number of faces
    hfFaces.detectedNum = static_cast<HInt32>(multipleFaceData.size());

    // Use vectors for RAII-safe memory management
    std::vector<HFaceRect> rects(hfFaces.detectedNum);
    std::vector<HInt32> trackIds(hfFaces.detectedNum);
    std::vector<HInt32> trackCounts(hfFaces.detectedNum);
    std::vector<HFloat> detConfidence(hfFaces.detectedNum);
    std::vector<HFFaceBasicToken> tokens(hfFaces.detectedNum);
    std::vector<HFloat> roll(hfFaces.detectedNum);
    std::vector<HFloat> yaw(hfFaces.detectedNum);
    std::vector<HFloat> pitch(hfFaces.detectedNum);

    if (hfFaces.detectedNum > 0)
    {
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        rects[i].x = static_cast<HInt32>(multipleFaceData[i].rect.x);
        rects[i].y = static_cast<HInt32>(multipleFaceData[i].rect.y);
        rects[i].width = static_cast<HInt32>(multipleFaceData[i].rect.width);
        rects[i].height = static_cast<HInt32>(multipleFaceData[i].rect.height);

        trackIds[i] = static_cast<HInt32>(multipleFaceData[i].trackId);
        trackCounts[i] = static_cast<HInt32>(multipleFaceData[i].trackCount);
        detConfidence[i] = static_cast<HFloat>(multipleFaceData[i].detConfidence);

        roll[i] = static_cast<HFloat>(multipleFaceData[i].angle.roll);
        yaw[i] = static_cast<HFloat>(multipleFaceData[i].angle.yaw);
        pitch[i] = static_cast<HFloat>(multipleFaceData[i].angle.pitch);

        tokens[i].size = static_cast<HInt32>(multipleFaceData[i].token->size());
        tokens[i].data = multipleFaceData[i].token->data();
      }

      hfFaces.rects = rects.data();
      hfFaces.trackIds = trackIds.data();
      hfFaces.trackCounts = trackCounts.data();
      hfFaces.detConfidence = detConfidence.data();
      hfFaces.tokens = tokens.data();
      hfFaces.angles.roll = roll.data();
      hfFaces.angles.yaw = yaw.data();
      hfFaces.angles.pitch = pitch.data();
    }

    // Process the faces
    HResult result = nitroImageStream->withNativeHandle([&](HFImageStream h)
                                                       { return HFMultipleFacePipelineProcess(_session, h, &hfFaces, hfParam); });

    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to process faces in pipeline, error code: %ld", result);
      return false;
    }

    return true;
  }

  std::vector<double> HybridSession::getRGBLivenessConfidence()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<double>();
    }

    // Get RGB liveness confidence
    HFRGBLivenessConfidence confidence = {};
    HResult result = HFGetRGBLivenessConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get RGB liveness confidence, error code: %ld", result);
      return std::vector<double>();
    }

    // Convert to vector
    std::vector<double> confidenceValues;
    if (confidence.num > 0 && confidence.confidence != nullptr)
    {
      confidenceValues.reserve(confidence.num);
      for (int i = 0; i < confidence.num; i++)
      {
        confidenceValues.push_back(static_cast<double>(confidence.confidence[i]));
      }
    }

    return confidenceValues;
  }

  std::vector<double> HybridSession::getFaceQualityConfidence()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<double>();
    }

    // Get face quality confidence
    HFFaceQualityConfidence confidence = {};
    HResult result = HFGetFaceQualityConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face quality confidence, error code: %ld", result);
      return std::vector<double>();
    }

    // Convert to vector
    std::vector<double> confidenceValues;
    if (confidence.num > 0 && confidence.confidence != nullptr)
    {
      confidenceValues.reserve(confidence.num);
      for (int i = 0; i < confidence.num; i++)
      {
        confidenceValues.push_back(static_cast<double>(confidence.confidence[i]));
      }
    }

    return confidenceValues;
  }

  std::vector<double> HybridSession::getFaceMaskConfidence()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<double>();
    }

    // Get face mask confidence
    HFFaceMaskConfidence confidence = {};
    HResult result = HFGetFaceMaskConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face mask confidence, error code: %ld", result);
      return std::vector<double>();
    }

    // Convert to vector
    std::vector<double> confidenceValues;
    if (confidence.num > 0 && confidence.confidence != nullptr)
    {
      confidenceValues.reserve(confidence.num);
      for (int i = 0; i < confidence.num; i++)
      {
        confidenceValues.push_back(static_cast<double>(confidence.confidence[i]));
      }
    }

    return confidenceValues;
  }

  std::vector<FaceInteractionState> HybridSession::getFaceInteractionState()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<FaceInteractionState>();
    }

    // Get face interaction state
    HFFaceInteractionState state = {};
    HResult result = HFGetFaceInteractionStateResult(_session, &state);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face interaction state, error code: %ld", result);
      return std::vector<FaceInteractionState>();
    }

    // Convert to vector
    std::vector<FaceInteractionState> stateValues;
    if (state.num > 0 && state.leftEyeStatusConfidence != nullptr && state.rightEyeStatusConfidence != nullptr)
    {
      stateValues.reserve(state.num);
      for (int i = 0; i < state.num; i++)
      {
        stateValues.push_back(FaceInteractionState(
            static_cast<double>(state.leftEyeStatusConfidence[i]),
            static_cast<double>(state.rightEyeStatusConfidence[i])));
      }
    }

    return stateValues;
  }

  std::vector<FaceInteractionsAction> HybridSession::getFaceInteractionActionsResult()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<FaceInteractionsAction>();
    }

    // Get face interaction actions
    HFFaceInteractionsActions actions = {};
    HResult result = HFGetFaceInteractionActionsResult(_session, &actions);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face interaction actions, error code: %ld", result);
      return std::vector<FaceInteractionsAction>();
    }

    // Convert to vector
    std::vector<FaceInteractionsAction> actionValues;
    if (actions.num > 0 && actions.normal != nullptr && actions.shake != nullptr &&
        actions.jawOpen != nullptr && actions.headRaise != nullptr && actions.blink != nullptr)
    {
      actionValues.reserve(actions.num);
      for (int i = 0; i < actions.num; i++)
      {
        actionValues.push_back(FaceInteractionsAction(
            static_cast<double>(actions.normal[i]),
            static_cast<double>(actions.shake[i]),
            static_cast<double>(actions.jawOpen[i]),
            static_cast<double>(actions.headRaise[i]),
            static_cast<double>(actions.blink[i])));
      }
    }

    return actionValues;
  }

  std::vector<FaceAttributeResult> HybridSession::getFaceAttributeResult()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<FaceAttributeResult>();
    }

    // Get face attribute results
    HFFaceAttributeResult results = {};
    HResult result = HFGetFaceAttributeResult(_session, &results);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face attribute results, error code: %ld", result);
      return std::vector<FaceAttributeResult>();
    }

    // Convert to vector
    std::vector<FaceAttributeResult> attributeValues;
    if (results.num > 0 && results.ageBracket != nullptr && results.gender != nullptr && results.race != nullptr)
    {
      attributeValues.reserve(results.num);
      for (int i = 0; i < results.num; i++)
      {
        attributeValues.push_back(FaceAttributeResult(
            static_cast<double>(results.ageBracket[i]),
            static_cast<double>(results.gender[i]),
            static_cast<double>(results.race[i])));
      }
    }

    return attributeValues;
  }

  std::vector<FaceEmotionResult> HybridSession::getFaceEmotionResult()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      return std::vector<FaceEmotionResult>();
    }

    HFFaceEmotionResult results = {};
    HResult result = HFGetFaceEmotionResult(_session, &results);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to get face emotion results, error code: %ld", result);
      return std::vector<FaceEmotionResult>();
    }

    std::vector<FaceEmotionResult> emotionValues;
    if (results.num > 0 && results.emotion != nullptr)
    {
      emotionValues.reserve(results.num);
      for (int i = 0; i < results.num; i++)
      {
        emotionValues.push_back(FaceEmotionResult(
            static_cast<double>(results.emotion[i])));
      }
    }

    return emotionValues;
  }

  std::shared_ptr<HybridImageBitmapSpec> HybridSession::getFaceAlignmentImage(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "HybridSession", "HybridSession is not initialized");
      throw std::runtime_error("HybridSession is not initialized");
    }

    if (!imageStream)
    {
      throw std::runtime_error("Image stream is null");
    }

    auto nitroImageStream = std::dynamic_pointer_cast<HybridImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to HybridImageStream");
    }

    // Create face token struct
    HFFaceBasicToken token = {};
    token.size = static_cast<HInt32>(faceToken->size());
    token.data = faceToken->data();

    // Get aligned image
    HFImageBitmap alignedBitmap = nullptr;
    HResult result = nitroImageStream->withNativeHandle([&](HFImageStream h)
                                                       { return HFFaceGetFaceAlignmentImage(_session, h, token, &alignedBitmap); });

    if (result != HSUCCEED || alignedBitmap == nullptr)
    {
      throw std::runtime_error("Failed to get face alignment image with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(alignedBitmap);
  }

  double HybridSession::getTrackPreviewSize()
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    HInt32 previewSize = 0;
    HResult result = HFSessionGetTrackPreviewSize(_session, &previewSize);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get track preview size with error code: " + std::to_string(result));
    }
    return static_cast<double>(previewSize);
  }

  double HybridSession::faceQualityDetect(const std::shared_ptr<ArrayBuffer> &faceToken)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    if (!faceToken || faceToken->size() == 0)
    {
      throw std::runtime_error("Invalid face token data");
    }

    HFFaceBasicToken token = {};
    token.size = static_cast<HInt32>(faceToken->size());
    token.data = faceToken->data();

    HFloat confidence = 0;
    HResult result = HFFaceQualityDetect(_session, token, &confidence);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to detect face quality with error code: " + std::to_string(result));
    }
    return static_cast<double>(confidence);
  }

  std::shared_ptr<ArrayBuffer> HybridSession::extractFaceFeatureFromAlignmentImage(const std::shared_ptr<HybridImageStreamSpec> &imageStream)
  {
    std::lock_guard<std::mutex> lock(_sessionMutex);
    if (_session == nullptr)
    {
      throw std::runtime_error("HybridSession is not initialized");
    }

    if (!imageStream)
    {
      throw std::runtime_error("Image stream is null");
    }

    auto nitroImageStream = std::dynamic_pointer_cast<HybridImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to HybridImageStream");
    }

    // Get expected feature length and pre-allocate
    HInt32 expectedLength = 0;
    HResult lengthResult = HFGetFeatureLength(&expectedLength);
    if (lengthResult != HSUCCEED)
    {
      throw std::runtime_error("Failed to get feature length");
    }

    HFFaceFeature feature = {};
    feature.size = expectedLength;
    std::vector<float> featureData(expectedLength);
    feature.data = featureData.data();

    HResult result = nitroImageStream->withNativeHandle([&](HFImageStream h)
                                                       { return HFFaceFeatureExtractWithAlignmentImage(_session, h, feature); });
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to extract face feature from alignment image with error code: " + std::to_string(result));
    }

    return ArrayBuffer::copy(
        reinterpret_cast<uint8_t *>(feature.data),
        feature.size * sizeof(float));
  }

  void HybridSession::reconfigure(const SessionCustomParameter &parameter, DetectMode detectMode, double maxDetectFaceNum, double detectPixelLevel, double trackByDetectModeFPS)
  {
    // Exclusive lock: reconfigure frees and replaces the handle.
    std::lock_guard<std::mutex> lock(_sessionMutex);
    // Release the existing session
    if (_session != nullptr)
    {
      HFReleaseInspireFaceSession(_session);
      _session = nullptr;
    }

    // Convert parameters
    HFSessionCustomParameter hfParam;
    hfParam.enable_recognition = parameter.enableRecognition.value_or(false) ? 1 : 0;
    hfParam.enable_liveness = parameter.enableLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_ir_liveness = parameter.enableIrLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_mask_detect = parameter.enableMaskDetect.value_or(false) ? 1 : 0;
    hfParam.enable_face_quality = parameter.enableFaceQuality.value_or(false) ? 1 : 0;
    hfParam.enable_face_attribute = parameter.enableFaceAttribute.value_or(false) ? 1 : 0;
    hfParam.enable_interaction_liveness = parameter.enableInteractionLiveness.value_or(false) ? 1 : 0;
    hfParam.enable_detect_mode_landmark = parameter.enableDetectModeLandmark.value_or(false) ? 1 : 0;
    hfParam.enable_face_pose = parameter.enableFacePose.value_or(false) ? 1 : 0;
    hfParam.enable_face_emotion = parameter.enableFaceEmotion.value_or(false) ? 1 : 0;

    // Create new session with updated config
    HResult result = HFCreateInspireFaceSession(
        hfParam,
        static_cast<HFDetectMode>(detectMode),
        static_cast<HInt32>(maxDetectFaceNum),
        static_cast<HInt32>(detectPixelLevel),
        static_cast<HInt32>(trackByDetectModeFPS),
        &_session);

    if (result != HSUCCEED || _session == nullptr)
    {
      throw std::runtime_error("Failed to reconfigure session with error code: " + std::to_string(result));
    }
  }

  size_t HybridSession::getExternalMemorySize() noexcept
  {
    // Rough estimate of the native memory pinned by a live session (MNN
    // interpreter + model tensors + tracking state). The exact size depends on
    // the model pack and enabled features; what matters is that Hermes sees a
    // multi-megabyte object instead of a few-byte wrapper, so unreferenced
    // sessions actually pressure the GC.
    return _session != nullptr ? 32 * 1024 * 1024 : 0;
  }

} // namespace margelo::nitro::nitroinspireface
