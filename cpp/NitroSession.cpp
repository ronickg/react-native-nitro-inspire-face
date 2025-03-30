#include "NitroSession.hpp"
#include <stdexcept>
#include <string>
#include <cstring>
#include <NitroModules/ArrayBuffer.hpp>
#include "FaceRect.hpp"
#include "FaceEulerAngle.hpp"
#include "FaceBasicToken.hpp"
#include "MultipleFaceData.hpp"
#include "NitroImageStream.hpp"

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
    // Note: angles might be single values for all faces, or arrays per face depending on API
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

} // namespace margelo::nitro::nitroinspireface
