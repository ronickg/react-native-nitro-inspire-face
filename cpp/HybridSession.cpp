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
#include "HybridImageStream.hpp"
#include <memory>
#include <vector>

namespace margelo::nitro::nitroinspireface
{
  HybridSession::HybridSession() : HybridObject(TAG), _session(nullptr) {}

  HybridSession::HybridSession(HFSession session) : HybridObject(TAG), _session(session) {}

  void HybridSession::cleanup()
  {
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

  std::vector<FaceData> HybridSession::executeFaceTrack(const std::shared_ptr<HybridImageStreamSpec> &imageStream)
  {
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

    HFImageStream nativeStream = nitroImageStream->getNativeHandle();

    HFMultipleFaceData results{};
    HResult status = HFExecuteFaceTrack(_session, nativeStream, &results);
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

        // Extract track ID and confidence
        double trackId = static_cast<double>(results.trackIds[i]);
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
        faceDataVector.emplace_back(rect, trackId, detConfidence, angles, buffer);
      }
    }

    return faceDataVector;
  }

  std::shared_ptr<ArrayBuffer> HybridSession::extractFaceFeature(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken)
  {
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
    HResult result = HFFaceFeatureExtract(_session, nitroImageStream->getNativeHandle(), token, &feature);

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
    hfParam.enable_recognition = parameter.enableRecognition ? 1 : 0;
    hfParam.enable_liveness = parameter.enableLiveness ? 1 : 0;
    hfParam.enable_ir_liveness = parameter.enableIrLiveness ? 1 : 0;
    hfParam.enable_mask_detect = parameter.enableMaskDetect ? 1 : 0;
    hfParam.enable_face_quality = parameter.enableFaceQuality ? 1 : 0;
    hfParam.enable_face_attribute = parameter.enableFaceAttribute ? 1 : 0;
    hfParam.enable_interaction_liveness = parameter.enableInteractionLiveness ? 1 : 0;
    hfParam.enable_detect_mode_landmark = parameter.enableDetectModeLandmark ? 1 : 0;

    // Convert vector<FaceData> to HFMultipleFaceData
    HFMultipleFaceData hfFaces = {};
    memset(&hfFaces, 0, sizeof(HFMultipleFaceData));

    // Set the number of faces
    hfFaces.detectedNum = static_cast<HInt32>(multipleFaceData.size());

    if (hfFaces.detectedNum > 0)
    {
      // Allocate arrays for face data
      hfFaces.rects = new HFaceRect[hfFaces.detectedNum];
      hfFaces.trackIds = new HInt32[hfFaces.detectedNum];
      hfFaces.detConfidence = new HFloat[hfFaces.detectedNum];
      hfFaces.tokens = new HFFaceBasicToken[hfFaces.detectedNum];

      // Allocate angle data
      hfFaces.angles.roll = new HFloat[hfFaces.detectedNum];
      hfFaces.angles.yaw = new HFloat[hfFaces.detectedNum];
      hfFaces.angles.pitch = new HFloat[hfFaces.detectedNum];

      // Copy data from FaceData vector to HFMultipleFaceData
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        // Copy rect
        hfFaces.rects[i].x = static_cast<HInt32>(multipleFaceData[i].rect.x);
        hfFaces.rects[i].y = static_cast<HInt32>(multipleFaceData[i].rect.y);
        hfFaces.rects[i].width = static_cast<HInt32>(multipleFaceData[i].rect.width);
        hfFaces.rects[i].height = static_cast<HInt32>(multipleFaceData[i].rect.height);

        // Copy track ID
        hfFaces.trackIds[i] = static_cast<HInt32>(multipleFaceData[i].trackId);

        // Copy detection confidence
        hfFaces.detConfidence[i] = static_cast<HFloat>(multipleFaceData[i].detConfidence);

        // Copy angles
        hfFaces.angles.roll[i] = static_cast<HFloat>(multipleFaceData[i].angle.roll);
        hfFaces.angles.yaw[i] = static_cast<HFloat>(multipleFaceData[i].angle.yaw);
        hfFaces.angles.pitch[i] = static_cast<HFloat>(multipleFaceData[i].angle.pitch);

        // Copy token
        hfFaces.tokens[i].size = static_cast<HInt32>(multipleFaceData[i].token->size());
        hfFaces.tokens[i].data = multipleFaceData[i].token->data();
      }
    }

    // Process the faces
    HResult result = HFMultipleFacePipelineProcess(_session, nitroImageStream->getNativeHandle(), &hfFaces, hfParam);

    // Clean up allocated memory
    if (hfFaces.rects)
      delete[] hfFaces.rects;
    if (hfFaces.trackIds)
      delete[] hfFaces.trackIds;
    if (hfFaces.detConfidence)
      delete[] hfFaces.detConfidence;
    if (hfFaces.tokens)
      delete[] hfFaces.tokens;
    if (hfFaces.angles.roll)
      delete[] hfFaces.angles.roll;
    if (hfFaces.angles.yaw)
      delete[] hfFaces.angles.yaw;
    if (hfFaces.angles.pitch)
      delete[] hfFaces.angles.pitch;

    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "HybridSession", "Failed to process faces in pipeline, error code: %ld", result);
      return false;
    }

    return true;
  }

  std::vector<double> HybridSession::getRGBLivenessConfidence()
  {
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

  std::shared_ptr<HybridImageBitmapSpec> HybridSession::getFaceAlignmentImage(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken)
  {
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
    HResult result = HFFaceGetFaceAlignmentImage(_session, nitroImageStream->getNativeHandle(), token, &alignedBitmap);

    if (result != HSUCCEED || alignedBitmap == nullptr)
    {
      throw std::runtime_error("Failed to get face alignment image with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(alignedBitmap);
  }

} // namespace margelo::nitro::nitroinspireface
