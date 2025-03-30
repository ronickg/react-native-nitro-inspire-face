#include "NitroSession.hpp"
#include <stdexcept>
#include <string>
#include <cstring>
#include <NitroModules/ArrayBuffer.hpp>
#include "FaceRect.hpp"
#include "FaceEulerAngle.hpp"
#include "FaceBasicToken.hpp"
#include "FaceFeature.hpp"
#include "MultipleFaceData.hpp"
#include "NitroImageStream.hpp"
#include <memory>

namespace margelo::nitro::nitroinspireface
{
  NitroSession::~NitroSession()
  {
    if (_session != nullptr)
    {
      HFReleaseInspireFaceSession(_session);
      _session = nullptr;
    }
  }

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

  MultipleFaceData NitroSession::executeFaceTrack(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream)
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

    // Convert results to a MultipleFaceData object
    std::vector<FaceRect> rects;
    std::vector<double> trackIds;
    std::vector<double> detConfidence;
    std::vector<FaceBasicToken> tokens;

    // Only process faces if there were detections
    if (results.detectedNum > 0)
    {
      // Process face rectangles
      if (results.rects != nullptr)
      {
        for (int i = 0; i < results.detectedNum; i++)
        {
          rects.push_back(FaceRect(
              static_cast<double>(results.rects[i].x),
              static_cast<double>(results.rects[i].y),
              static_cast<double>(results.rects[i].width),
              static_cast<double>(results.rects[i].height)));
        }
      }

      // Process track IDs
      if (results.trackIds != nullptr)
      {
        for (int i = 0; i < results.detectedNum; i++)
        {
          trackIds.push_back(static_cast<double>(results.trackIds[i]));
        }
      }

      // Process detection confidence
      if (results.detConfidence != nullptr)
      {
        for (int i = 0; i < results.detectedNum; i++)
        {
          detConfidence.push_back(static_cast<double>(results.detConfidence[i]));
        }
      }

      // Process tokens
      if (results.tokens != nullptr)
      {
        for (int i = 0; i < results.detectedNum; i++)
        {
          // Get the token size
          int tokenSize = 0;
          if (results.tokens[i].size > 0)
          {
            tokenSize = static_cast<int>(results.tokens[i].size);
          }

          // Create a shared pointer to an ArrayBuffer
          std::shared_ptr<margelo::nitro::ArrayBuffer> buffer;

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

          // Create a FaceBasicToken
          if (buffer)
          {
            tokens.push_back(FaceBasicToken(
                static_cast<double>(tokenSize),
                buffer));
          }
        }
      }
    }

    // Create FaceEulerAngle from results
    double roll = 0.0, yaw = 0.0, pitch = 0.0;

    // Check if the angle pointers are valid before dereferencing
    if (results.angles.roll != nullptr)
    {
      roll = static_cast<double>(*results.angles.roll);
    }

    if (results.angles.yaw != nullptr)
    {
      yaw = static_cast<double>(*results.angles.yaw);
    }

    if (results.angles.pitch != nullptr)
    {
      pitch = static_cast<double>(*results.angles.pitch);
    }

    FaceEulerAngle angles(roll, yaw, pitch);

    // Return the MultipleFaceData
    return MultipleFaceData(
        static_cast<double>(results.detectedNum),
        rects,
        trackIds,
        detConfidence,
        angles,
        tokens);
  }

  FaceFeature NitroSession::extractFaceFeature(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const FaceBasicToken &faceToken)
  {
    if (!imageStream)
    {
      throw std::runtime_error("Invalid input parameters");
    }

    // Cast the image stream to NitroImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<NitroImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to NitroImageStream");
    }

    // Create face token struct and initialize it properly
    HFFaceBasicToken token = {};
    token.size = static_cast<HInt32>(faceToken.size);

    // Get the raw data from the ArrayBuffer
    auto buffer = faceToken.data;
    if (!buffer || buffer->size() == 0)
    {
      throw std::runtime_error("Invalid face token data");
    }
    token.data = reinterpret_cast<void *>(buffer->data());

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

    // Create ArrayBuffer and copy feature data
    auto featureBuffer = margelo::nitro::ArrayBuffer::allocate(feature.size * sizeof(float));
    std::memcpy(featureBuffer->data(), feature.data, feature.size * sizeof(float));

    // Return FaceFeature directly (not a shared_ptr)
    return FaceFeature(static_cast<double>(feature.size), featureBuffer);
  }

  MultipleFaceData NitroSession::multipleFacePipelineProcess(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const MultipleFaceData &multipleFaceData, const SessionCustomParameter &parameter)
  {
    if (_session == nullptr)
    {
      throw std::runtime_error("Session is not initialized");
    }

    if (!imageStream)
    {
      throw std::runtime_error("Image stream is null");
    }

    // Cast the image stream to NitroImageStream
    auto nitroImageStream = std::dynamic_pointer_cast<NitroImageStream>(imageStream);
    if (!nitroImageStream)
    {
      throw std::runtime_error("Failed to cast to NitroImageStream");
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

    // Convert MultipleFaceData to HFMultipleFaceData
    HFMultipleFaceData hfFaces = {};
    memset(&hfFaces, 0, sizeof(HFMultipleFaceData));

    // Set the number of faces
    hfFaces.detectedNum = static_cast<HInt32>(multipleFaceData.detectedNum);

    // Allocate and copy face rects
    if (!multipleFaceData.rects.empty())
    {
      hfFaces.rects = new HFaceRect[hfFaces.detectedNum];
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        hfFaces.rects[i].x = static_cast<HInt32>(multipleFaceData.rects[i].x);
        hfFaces.rects[i].y = static_cast<HInt32>(multipleFaceData.rects[i].y);
        hfFaces.rects[i].width = static_cast<HInt32>(multipleFaceData.rects[i].width);
        hfFaces.rects[i].height = static_cast<HInt32>(multipleFaceData.rects[i].height);
      }
    }

    // Allocate and copy track IDs
    if (!multipleFaceData.trackIds.empty())
    {
      hfFaces.trackIds = new HInt32[hfFaces.detectedNum];
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        hfFaces.trackIds[i] = static_cast<HInt32>(multipleFaceData.trackIds[i]);
      }
    }

    // Allocate and copy detection confidence
    if (!multipleFaceData.detConfidence.empty())
    {
      hfFaces.detConfidence = new HFloat[hfFaces.detectedNum];
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        hfFaces.detConfidence[i] = static_cast<HFloat>(multipleFaceData.detConfidence[i]);
      }
    }

    // Allocate and copy face tokens
    if (!multipleFaceData.tokens.empty())
    {
      hfFaces.tokens = new HFFaceBasicToken[hfFaces.detectedNum];
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        hfFaces.tokens[i].size = static_cast<HInt32>(multipleFaceData.tokens[i].size);
        hfFaces.tokens[i].data = multipleFaceData.tokens[i].data->data();
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

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to process faces in pipeline with error code: " + std::to_string(result));
    }

    // Convert back to MultipleFaceData
    std::vector<FaceRect> rects;
    std::vector<double> trackIds;
    std::vector<double> detConfidence;
    std::vector<FaceBasicToken> tokens;

    // Process face rectangles
    if (hfFaces.rects != nullptr)
    {
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        rects.push_back(FaceRect(
            static_cast<double>(hfFaces.rects[i].x),
            static_cast<double>(hfFaces.rects[i].y),
            static_cast<double>(hfFaces.rects[i].width),
            static_cast<double>(hfFaces.rects[i].height)));
      }
    }

    // Process track IDs
    if (hfFaces.trackIds != nullptr)
    {
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        trackIds.push_back(static_cast<double>(hfFaces.trackIds[i]));
      }
    }

    // Process detection confidence
    if (hfFaces.detConfidence != nullptr)
    {
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        detConfidence.push_back(static_cast<double>(hfFaces.detConfidence[i]));
      }
    }

    // Process tokens
    if (hfFaces.tokens != nullptr)
    {
      for (int i = 0; i < hfFaces.detectedNum; i++)
      {
        int tokenSize = static_cast<int>(hfFaces.tokens[i].size);
        std::shared_ptr<margelo::nitro::ArrayBuffer> buffer;

        if (tokenSize > 0 && hfFaces.tokens[i].data != nullptr)
        {
          try
          {
            buffer = margelo::nitro::ArrayBuffer::copy(
                static_cast<uint8_t *>(hfFaces.tokens[i].data),
                tokenSize);
          }
          catch (const std::exception &e)
          {
            buffer = margelo::nitro::ArrayBuffer::allocate(0);
          }
        }
        else
        {
          buffer = margelo::nitro::ArrayBuffer::allocate(0);
        }

        if (buffer)
        {
          tokens.push_back(FaceBasicToken(
              static_cast<double>(tokenSize),
              buffer));
        }
      }
    }

    // Create FaceEulerAngle from results
    double roll = 0.0, yaw = 0.0, pitch = 0.0;
    if (hfFaces.angles.roll != nullptr)
      roll = static_cast<double>(*hfFaces.angles.roll);
    if (hfFaces.angles.yaw != nullptr)
      yaw = static_cast<double>(*hfFaces.angles.yaw);
    if (hfFaces.angles.pitch != nullptr)
      pitch = static_cast<double>(*hfFaces.angles.pitch);

    FaceEulerAngle angles(roll, yaw, pitch);

    return MultipleFaceData(
        static_cast<double>(hfFaces.detectedNum),
        rects,
        trackIds,
        detConfidence,
        angles,
        tokens);
  }

} // namespace margelo::nitro::nitroinspireface
