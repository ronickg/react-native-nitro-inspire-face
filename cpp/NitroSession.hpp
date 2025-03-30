#pragma once

#include "HybridNitroSessionSpec.hpp"
#include "HybridNitroImageStreamSpec.hpp"
#include "MultipleFaceData.hpp"
#include "FaceInteractionState.hpp"
#include "FaceInteractionsAction.hpp"
#include "FaceAttributeResult.hpp"
#include "inspireface.h"

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the Session module
   */
  class NitroSession : public virtual HybridNitroSessionSpec
  {
  public:
    // Default constructor required for autolink
    NitroSession() : HybridObject(TAG), _session(nullptr) {}

    // Constructor with session
    NitroSession(HFSession session) : HybridObject(TAG), _session(session) {}

    // Destructor
    ~NitroSession() override
    {
      if (_session != nullptr)
      {
        HFReleaseInspireFaceSession(_session);
        _session = nullptr;
      }
    }

  private:
    static constexpr auto TAG = "NitroSession";

  public:
    // Methods
    void setTrackPreviewSize(double size) override;
    void setFaceDetectThreshold(double threshold) override;
    void setFilterMinimumFacePixelSize(double size) override;
    MultipleFaceData executeFaceTrack(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream) override;
    FaceFeature extractFaceFeature(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const FaceBasicToken &faceToken) override;
    bool multipleFacePipelineProcess(const std::shared_ptr<HybridNitroImageStreamSpec> &imageStream, const MultipleFaceData &multipleFaceData, const SessionCustomParameter &parameter) override;
    std::vector<double> getRGBLivenessConfidence() override;
    std::vector<double> getFaceQualityConfidence() override;
    std::vector<double> getFaceMaskConfidence() override;
    std::vector<FaceInteractionState> getFaceInteractionState() override;
    std::vector<FaceInteractionsAction> getFaceInteractionActionsResult() override;
    std::vector<FaceAttributeResult> getFaceAttributeResult() override;

  private:
    HFSession _session;
  };

} // namespace margelo::nitro::nitroinspireface
