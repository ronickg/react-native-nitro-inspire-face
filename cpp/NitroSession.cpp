#include "NitroSession.hpp"
#include <stdexcept>
#include <string>

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

} // namespace margelo::nitro::nitroinspireface
