#pragma once

#include "HybridInspireFaceSpec.hpp"
#include "FeatureHubConfiguration.hpp"
#include "SessionCustomParameter.hpp"
#include "DetectMode.hpp"
#include "CameraRotation.hpp"
#include "HybridImageBitmap.hpp"
#include "SearchMode.hpp"
#include "PrimaryKeyMode.hpp"
#include "HybridSession.hpp"
#include "HybridImageStream.hpp"
#include "inspireface.h"
#include "HybridAssetManagerSpec.hpp"
#include <NitroModules/ArrayBuffer.hpp>
#include <string>
#include <memory>
#include <vector>
#include <optional>

namespace margelo::nitro::nitroinspireface
{

  /**
   * Implementation of the HybridInspireFace module
   */
  class HybridInspireFace : public virtual HybridInspireFaceSpec
  {
  public:
    // Constructor
    HybridInspireFace();

    // Destructor
    ~HybridInspireFace() override = default;

  private:
    std::shared_ptr<HybridAssetManagerSpec> assetManager;

  public:
    std::string getVersion() override;
    bool launch(const std::string &path) override;
    bool featureHubDataEnable(const FeatureHubConfiguration &config) override;
    bool featureHubFaceSearchThresholdSetting(double threshold) override;
    std::shared_ptr<HybridSessionSpec> createSession(
        const SessionCustomParameter &parameter,
        DetectMode detectMode,
        double maxDetectFaceNum,
        double detectPixelLevel,
        double trackByDetectModeFPS) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmapFromFilePath(double channels, const std::string &filePath) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmapFromBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height, double channels) override;
    std::shared_ptr<HybridImageStreamSpec> createImageStreamFromBitmap(const std::shared_ptr<HybridImageBitmapSpec> &bitmap, CameraRotation rotation) override;
    std::vector<Point2f> getFaceDenseLandmarkFromFaceToken(const std::shared_ptr<ArrayBuffer> &token) override;
    double featureHubFaceInsert(const FaceFeatureIdentity &feature) override;
    bool featureHubFaceUpdate(const FaceFeatureIdentity &feature) override;
    bool featureHubFaceRemove(double id) override;
    std::optional<FaceFeatureIdentity> featureHubFaceSearch(const std::vector<double> &feature) override;
    std::optional<FaceFeatureIdentity> featureHubGetFaceIdentity(double id) override;
    std::vector<SearchTopKResult> featureHubFaceSearchTopK(const std::vector<double> &feature, double topK) override;
    double getFeatureLength() override;
    double faceComparison(const std::vector<double> &feature1, const std::vector<double> &feature2) override;
  };

} // namespace margelo::nitro::nitroinspireface
