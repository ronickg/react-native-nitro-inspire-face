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
#include <mutex>
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
    double getTrackPreviewSize() override;
    double faceQualityDetect(const std::shared_ptr<ArrayBuffer> &faceToken) override;
    std::shared_ptr<ArrayBuffer> extractFaceFeatureFromAlignmentImage(const std::shared_ptr<HybridImageStreamSpec> &imageStream) override;
    void reconfigure(const SessionCustomParameter &parameter, DetectMode detectMode, double maxDetectFaceNum, double detectPixelLevel, double trackByDetectModeFPS) override;

    // Memory pressure hint for the JS GC: a session pins an MNN interpreter +
    // model tensors natively while the JS wrapper is only a few bytes.
    size_t getExternalMemorySize() noexcept override;

  private:
    HFSession _session;
    // Guards _session against use-during-free: JS callers may invoke methods
    // from multiple runtimes/threads (e.g. a VisionCamera frame-processor
    // worklet) while dispose()/reconfigure() runs on another thread. Methods
    // take this lock for the full duration of their native call; dispose()/
    // reconfigure() take it too, so releasing the native session waits for any
    // in-flight call instead of freeing the MNN interpreter under it
    // (EXC_BAD_ACCESS). Exclusive (not shared): executeFaceTrack /
    // multipleFacePipelineProcess / extractFaceFeature all WRITE session-internal
    // buffers, so two concurrent calls on one session would corrupt them.
    mutable std::mutex _sessionMutex;
  };

} // namespace margelo::nitro::nitroinspireface
