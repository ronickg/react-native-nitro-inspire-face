#pragma once

#include "HybridSessionSpec.hpp"
#include "HybridImageStreamSpec.hpp"
#include "HybridImageBitmap.hpp"
#include "SessionCustomParameter.hpp"
#include "DetectMode.hpp"
#include "FaceInteractionState.hpp"
#include "FaceInteractionsAction.hpp"
#include "FaceAttributeResult.hpp"
#include "FaceEmotionResult.hpp"
#include "FaceData.hpp"
#include "inspireface.h"
#include <NitroModules/ArrayBuffer.hpp>
#include <vector>

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the HybridSession module
   */
  class HybridSession : public virtual HybridSessionSpec
  {
  public:
    // Default constructor required for autolink
    HybridSession();

    // Constructor with session
    HybridSession(HFSession session);

    // Destructor
    ~HybridSession() override;

    // Override dispose to clean up resources
    void dispose() override;

  private:
    // Private cleanup method used by both dispose and destructor
    void cleanup();

  public:
    // Methods
    void setTrackPreviewSize(double size) override;
    void setFaceDetectThreshold(double threshold) override;
    void setFilterMinimumFacePixelSize(double size) override;
    void setTrackModeSmoothRatio(double ratio) override;
    void setTrackModeNumSmoothCacheFrame(double num) override;
    void setTrackModeDetectInterval(double num) override;
    void setTrackLostRecoveryMode(bool enable) override;
    void setLightTrackConfidenceThreshold(double value) override;
    void clearTrackingFace() override;
    std::vector<FaceData> executeFaceTrack(const std::shared_ptr<HybridImageStreamSpec> &imageStream) override;
    std::shared_ptr<ArrayBuffer> extractFaceFeature(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken) override;
    bool multipleFacePipelineProcess(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::vector<FaceData> &multipleFaceData, const SessionCustomParameter &parameter) override;
    std::vector<double> getRGBLivenessConfidence() override;
    std::vector<double> getFaceQualityConfidence() override;
    std::vector<double> getFaceMaskConfidence() override;
    std::vector<FaceInteractionState> getFaceInteractionState() override;
    std::vector<FaceInteractionsAction> getFaceInteractionActionsResult() override;
    std::vector<FaceAttributeResult> getFaceAttributeResult() override;
    std::vector<FaceEmotionResult> getFaceEmotionResult() override;
    std::shared_ptr<HybridImageBitmapSpec> getFaceAlignmentImage(const std::shared_ptr<HybridImageStreamSpec> &imageStream, const std::shared_ptr<ArrayBuffer> &faceToken) override;
    void reconfigure(const SessionCustomParameter &parameter, DetectMode detectMode, double maxDetectFaceNum, double detectPixelLevel, double trackByDetectModeFPS) override;

  private:
    HFSession _session;
  };

} // namespace margelo::nitro::nitroinspireface
