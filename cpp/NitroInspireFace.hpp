#pragma once

// These headers are provided by the build system
#include "HybridNitroInspireFaceSpec.hpp"
#include "FeatureHubConfiguration.hpp"
#include "SessionCustomParameter.hpp"
#include "DetectMode.hpp"
#include "NitroSession.hpp"
#include "inspireface.h"

namespace margelo::nitro::nitroinspireface
{

  /**
   * Implementation of the NitroInspireFace module
   */
  class NitroInspireFace : public virtual HybridNitroInspireFaceSpec
  {
  public:
    // Constructor
    NitroInspireFace() : HybridObject(TAG) {}

    // Destructor
    ~NitroInspireFace() override = default;

    // Methods
    double multiply(double a, double b) override;
    std::string getVersion() override;
    void launch(const std::string &path) override;
    void featureHubDataEnable(const FeatureHubConfiguration &config) override;
    void featureHubFaceSearchThresholdSetting(double threshold) override;
    std::shared_ptr<HybridSessionSpec> createSession(
        const SessionCustomParameter &parameter,
        DetectMode detectMode,
        double maxDetectFaceNum,
        double detectPixelLevel,
        double trackByDetectModeFPS) override;
  };

} // namespace margelo::nitro::nitroinspireface
