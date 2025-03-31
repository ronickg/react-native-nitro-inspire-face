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
#include <optional>

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
    std::string getVersion() override;
    bool launch(const std::string &path) override;
    bool featureHubDataEnable(const FeatureHubConfiguration &config) override;
    bool featureHubFaceSearchThresholdSetting(double threshold) override;
    std::shared_ptr<HybridNitroSessionSpec> createSession(
        const SessionCustomParameter &parameter,
        DetectMode detectMode,
        double maxDetectFaceNum,
        double detectPixelLevel,
        double trackByDetectModeFPS) override;
    ImageBitmap createImageBitmapFromFilePath(double channels, const std::string &filePath) override;
    ImageBitmap createImageBitmapFromBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height, double channels) override;
    std::shared_ptr<HybridNitroImageStreamSpec> createImageStreamFromBitmap(const ImageBitmap &bitmap, CameraRotation rotation) override;
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
