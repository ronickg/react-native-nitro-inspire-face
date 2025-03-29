#pragma once

#include "../nitrogen/generated/shared/c++/HybridSessionSpec.hpp"
#include "../android/libs/include/inspireface.h"

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the Session module
   */
  class NitroSession : public virtual HybridSessionSpec
  {
  public:
    // Default constructor required for autolink
    NitroSession() : HybridObject(TAG), _session(nullptr) {}

    // Constructor with session
    NitroSession(HFSession session) : HybridObject(TAG), _session(session) {}

    // Destructor
    ~NitroSession() override;

    // Methods
    void setTrackPreviewSize(double size) override;
    void setFaceDetectThreshold(double threshold) override;
    void setFilterMinimumFacePixelSize(double size) override;

  private:
    HFSession _session;
  };

} // namespace margelo::nitro::nitroinspireface
