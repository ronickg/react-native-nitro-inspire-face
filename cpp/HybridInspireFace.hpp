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
#include <NitroModules/NitroLogger.hpp>
#include "FaceFeatureIdentity.hpp"
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
    double getFeatureLength() override;
    double getFaceDenseLandmarkLength() override;
    double getFaceBasicTokenLength() override;
    void launch(const std::string &path) override;
    void reload(const std::string &path) override;
    void terminate() override;
    void featureHubDataEnable(const FeatureHubConfiguration &config) override;
    void featureHubDataDisable() override;
    void featureHubFaceSearchThresholdSetting(double threshold) override;
    std::shared_ptr<HybridSessionSpec> createSession(
        const SessionCustomParameter &parameter,
        DetectMode detectMode,
        double maxDetectFaceNum,
        double detectPixelLevel,
        double trackByDetectModeFPS) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmapFromFilePath(double channels, const std::string &filePath) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmapFromBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height, double channels) override;
    std::shared_ptr<HybridImageStreamSpec> createImageStreamFromBitmap(const std::shared_ptr<HybridImageBitmapSpec> &bitmap, CameraRotation rotation) override;
    std::vector<Point2f> getFaceDenseLandmarkFromFaceToken(const std::shared_ptr<ArrayBuffer> &token, std::optional<double> num = std::nullopt) override;
    std::vector<Point2f> getFaceFiveKeyPointsFromFaceToken(const std::shared_ptr<ArrayBuffer> &token, std::optional<double> num = std::nullopt) override;
    double featureHubFaceInsert(const FaceFeatureIdentity &feature) override;
    bool featureHubFaceUpdate(const FaceFeatureIdentity &feature) override;
    bool featureHubFaceRemove(double id) override;
    std::optional<FaceFeatureIdentity> featureHubFaceSearch(const std::shared_ptr<ArrayBuffer> &feature) override;
    std::optional<FaceFeatureIdentity> featureHubGetFaceIdentity(double id) override;
    std::vector<SearchTopKResult> featureHubFaceSearchTopK(const std::shared_ptr<ArrayBuffer> &feature, double topK) override;
    double featureHubGetFaceCount() override;
    std::vector<double> featureHubGetExistingIds() override;
    double faceComparison(const std::shared_ptr<ArrayBuffer> &feature1, const std::shared_ptr<ArrayBuffer> &feature2) override;
    double getRecommendedCosineThreshold() override;
    double cosineSimilarityConvertToPercentage(double similarity) override;
    void updateCosineSimilarityConverter(const SimilarityConverterConfig &config) override;
    SimilarityConverterConfig getCosineSimilarityConverter() override;
    void setExpansiveHardwareRockchipDmaHeapPath(const std::string &path) override;
    std::string queryExpansiveHardwareRockchipDmaHeapPath() override;
    void setAppleCoreMLInferenceMode(AppleCoreMLInferenceMode mode) override;
    void setCudaDeviceId(double deviceId) override;
    double getCudaDeviceId() override;
    void printCudaDeviceInfo() override;
    double getNumCudaDevices() override;
    bool checkCudaDeviceSupport() override;
  };

} // namespace margelo::nitro::nitroinspireface
