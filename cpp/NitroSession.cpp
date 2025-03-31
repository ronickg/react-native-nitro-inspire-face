#include "NitroSession.hpp"
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
#include "NitroImageStream.hpp"
#include <memory>
#include <vector>

namespace margelo::nitro::nitroinspireface
{
  void NitroSession::setTrackPreviewSize(double size)
  {
    if (_session == nullptr)
    {
      throw std::runtime_error("Session is not initialized");
    }

    HResult result = HFSessionSetTrackPreviewSize(_session, static_cast<HInt32>(size));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set track preview size with error code: " + std::to_string(result));
    }
  }

  void NitroSession::setFaceDetectThreshold(double threshold)
  {
    if (_session == nullptr)
    {
      throw std::runtime_error("Session is not initialized");
    }

    HResult result = HFSessionSetFaceDetectThreshold(_session, static_cast<HFloat>(threshold));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set face detect threshold with error code: " + std::to_string(result));
    }
  }

  void NitroSession::setFilterMinimumFacePixelSize(double size)
  {
    if (_session == nullptr)
    {
      throw std::runtime_error("Session is not initialized");
    }

    HResult result = HFSessionSetFilterMinimumFacePixelSize(_session, static_cast<HInt32>(size));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set minimum face pixel size with error code: " + std::to_string(result));
    }
  }

  std::vector<FaceData> NitroSession::executeFaceTrack(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream)
  {
    if (_session == nullptr)
    {
      throw std::runtime_error("Session is not initialized");
    }

    if (!imageStream)
    {
      throw std::runtime_error("Image stream is null");
    }

    // Create a struct to store the results
    HFMultipleFaceData results = {};
    memset(&results, 0, sizeof(HFMultipleFaceData));

    // Get the native image stream handle - we need to cast to the correct derived class
    auto nitroImageStream = std::dynamic_pointer_cast<NitroImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to NitroImageStream");
    }

    HFImageStream streamHandle = nitroImageStream->getNativeHandle();
    if (!streamHandle)
    {
      throw std::runtime_error("Native image stream handle is null");
    }

    // Execute face tracking
    HResult result = HFExecuteFaceTrack(_session, streamHandle, &results);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to execute face track with error code: " + std::to_string(result));
    }

    // Convert results to vector of FaceData objects
    std::vector<FaceData> faceDataVector;

    // Only process faces if there were detections
    if (results.detectedNum > 0)
    {
      faceDataVector.reserve(results.detectedNum);

      for (int i = 0; i < results.detectedNum; i++)
      {
        // Process face rectangles
        FaceRect rect(0, 0, 0, 0);
        if (results.rects != nullptr)
        {
          rect = FaceRect(
              static_cast<double>(results.rects[i].x),
              static_cast<double>(results.rects[i].y),
              static_cast<double>(results.rects[i].width),
              static_cast<double>(results.rects[i].height));
        }

        // Process track IDs
        double trackId = 0;
        if (results.trackIds != nullptr)
        {
          trackId = static_cast<double>(results.trackIds[i]);
        }

        // Process detection confidence
        double detConfidence = 0;
        if (results.detConfidence != nullptr)
        {
          detConfidence = static_cast<double>(results.detConfidence[i]);
        }

        // Process Euler angles
        double roll = 0.0, yaw = 0.0, pitch = 0.0;
        if (results.angles.roll != nullptr)
        {
          roll = static_cast<double>(results.angles.roll[i]);
        }
        if (results.angles.yaw != nullptr)
        {
          yaw = static_cast<double>(results.angles.yaw[i]);
        }
        if (results.angles.pitch != nullptr)
        {
          pitch = static_cast<double>(results.angles.pitch[i]);
        }
        FaceEulerAngle angles(roll, yaw, pitch);

        // Process token
        std::shared_ptr<margelo::nitro::ArrayBuffer> buffer;
        if (results.tokens != nullptr)
        {
          // Get the token size
          int tokenSize = 0;
          if (results.tokens[i].size > 0)
          {
            tokenSize = static_cast<int>(results.tokens[i].size);
          }

          if (tokenSize > 0 && results.tokens[i].data != nullptr)
          {
            try
            {
              // Create a new ArrayBuffer with the token data
              buffer = margelo::nitro::ArrayBuffer::copy(
                  static_cast<uint8_t *>(results.tokens[i].data),
                  tokenSize);
            }
            catch (const std::exception &e)
            {
              // If copying fails, fall back to an empty buffer
              buffer = margelo::nitro::ArrayBuffer::allocate(0);
            }
          }
          else
          {
            // Create an empty buffer if no data
            buffer = margelo::nitro::ArrayBuffer::allocate(0);
          }
        }
        else
        {
          buffer = margelo::nitro::ArrayBuffer::allocate(0);
        }

        // Add the FaceData to the vector
        faceDataVector.emplace_back(rect, trackId, detConfidence, angles, buffer);
      }
    }

    return faceDataVector;
  }

  std::vector<double> NitroSession::extractFaceFeature(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken)
  {
    if (!imageStream || !faceToken)
    {
      throw std::runtime_error("Invalid input parameters");
    }

    // Cast the image stream to NitroImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<NitroImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to NitroImageStream");
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

    // Convert the float array to a vector of doubles
    std::vector<double> featureVector;
    featureVector.reserve(feature.size);
    for (int i = 0; i < feature.size; i++)
    {
      featureVector.push_back(static_cast<double>(static_cast<float *>(feature.data)[i]));
    }

    return featureVector;
  }

  bool NitroSession::multipleFacePipelineProcess(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const std::vector<FaceData> &multipleFaceData, const SessionCustomParameter &parameter)
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return false;
    }

    if (!imageStream)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Image stream is null");
      return false;
    }

    // Cast the image stream to NitroImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<NitroImageStream>(imageStream);
    if (!nitroImageStream)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to cast to NitroImageStream");
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
      Logger::log(LogLevel::Error, "NitroSession", "Failed to process faces in pipeline, error code: %ld", result);
      return false;
    }

    return true;
  }

  std::vector<double> NitroSession::getRGBLivenessConfidence()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<double>();
    }

    // Get RGB liveness confidence
    HFRGBLivenessConfidence confidence = {};
    HResult result = HFGetRGBLivenessConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get RGB liveness confidence, error code: %ld", result);
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

  std::vector<double> NitroSession::getFaceQualityConfidence()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<double>();
    }

    // Get face quality confidence
    HFFaceQualityConfidence confidence = {};
    HResult result = HFGetFaceQualityConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get face quality confidence, error code: %ld", result);
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

  std::vector<double> NitroSession::getFaceMaskConfidence()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<double>();
    }

    // Get face mask confidence
    HFFaceMaskConfidence confidence = {};
    HResult result = HFGetFaceMaskConfidence(_session, &confidence);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get face mask confidence, error code: %ld", result);
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

  std::vector<FaceInteractionState> NitroSession::getFaceInteractionState()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<FaceInteractionState>();
    }

    // Get face interaction state
    HFFaceInteractionState state = {};
    HResult result = HFGetFaceInteractionStateResult(_session, &state);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get face interaction state, error code: %ld", result);
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

  std::vector<FaceInteractionsAction> NitroSession::getFaceInteractionActionsResult()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<FaceInteractionsAction>();
    }

    // Get face interaction actions
    HFFaceInteractionsActions actions = {};
    HResult result = HFGetFaceInteractionActionsResult(_session, &actions);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get face interaction actions, error code: %ld", result);
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

  std::vector<FaceAttributeResult> NitroSession::getFaceAttributeResult()
  {
    if (_session == nullptr)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Session is not initialized");
      return std::vector<FaceAttributeResult>();
    }

    // Get face attribute results
    HFFaceAttributeResult results = {};
    HResult result = HFGetFaceAttributeResult(_session, &results);
    if (result != HSUCCEED)
    {
      Logger::log(LogLevel::Error, "NitroSession", "Failed to get face attribute results, error code: %ld", result);
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

} // namespace margelo::nitro::nitroinspireface
