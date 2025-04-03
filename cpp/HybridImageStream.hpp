#pragma once

#include "HybridImageStreamSpec.hpp"
#include "inspireface.h"
#include "ImageFormat.hpp"
#include "CameraRotation.hpp"
#include "HybridImageBitmapSpec.hpp"
#include <memory>
#include <optional>

namespace margelo::nitro::nitroinspireface
{
  class HybridImageBitmap;

  /**
   * Implementation of the HybridImageStream module
   */
  class HybridImageStream : public virtual HybridImageStreamSpec
  {
  public:
    // Default constructor required for autolink
    HybridImageStream();

    // Constructor with stream
    HybridImageStream(HFImageStream stream);

    // Destructor
    ~HybridImageStream() override;

    // Override dispose to clean up resources
    void dispose() override;

  private:
    // Private cleanup method used by both dispose and destructor
    void cleanup();

  public:
    // Methods
    void writeImageToFile(const std::string &filePath) override;
    void setFormat(ImageFormat format) override;
    void setRotation(CameraRotation rotation) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmap(std::optional<bool> isRotate = std::nullopt, std::optional<double> scale = std::nullopt) override;

    // Get the native stream handle
    HFImageStream getNativeHandle() const { return _stream; }

  private:
    HFImageStream _stream;
  };

} // namespace margelo::nitro::nitroinspireface
