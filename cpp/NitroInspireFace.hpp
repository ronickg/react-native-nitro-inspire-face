#pragma once

#include "HybridNitroInspireFaceSpec.hpp"
#include "FeatureHubConfiguration.hpp"
#include "SessionCustomParameter.hpp"
#include "DetectMode.hpp"
#include "CameraRotation.hpp"
#include "ImageBitmap.hpp"
#include "SearchMode.hpp"
#include "PrimaryKeyMode.hpp"
#include "NitroSession.hpp"
#include "NitroImageStream.hpp"
#include "inspireface.h"
#include <NitroModules/ArrayBuffer.hpp>
#include <string>
#include <memory>
#include <vector>

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

  private:
    static constexpr auto TAG = "NitroInspireFace";

  public:
    // Methods
    double multiply(double a, double b) override;
    std::string getVersion() override;
    void launch(const std::string &path) override;
    void featureHubDataEnable(const FeatureHubConfiguration &config) override;
    void featureHubFaceSearchThresholdSetting(double threshold) override;
    std::shared_ptr<HybridNitroSessionSpec> createSession(
        const SessionCustomParameter &parameter,
        DetectMode detectMode,
        double maxDetectFaceNum,
        double detectPixelLevel,
        double trackByDetectModeFPS) override;
    ImageBitmap createImageBitmapFromFilePath(double channels, const std::string &filePath) override;
    std::shared_ptr<HybridNitroImageStreamSpec> createImageStreamFromBitmap(const ImageBitmap &bitmap, CameraRotation rotation) override;
    std::vector<Point2f> getFaceDenseLandmarkFromFaceToken(const FaceBasicToken &token) override;
  };

} // namespace margelo::nitro::nitroinspireface
