#pragma once

#include "HybridNitroImageBitmapSpec.hpp"
#include "inspireface.h"
#include <NitroModules/ArrayBuffer.hpp>
#include <NitroModules/NitroLogger.hpp>
#include <memory>
#include <string>

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the ImageBitmap module
   */
  class NitroImageBitmap : public virtual HybridNitroImageBitmapSpec
  {
  public:
    // Default constructor required for autolink
    NitroImageBitmap();

    // Constructor with bitmap
    NitroImageBitmap(HFImageBitmap bitmap);

    // Destructor
    ~NitroImageBitmap() override;

    // Override dispose to clean up resources
    void dispose() override;

  private:
    // Private cleanup method used by both dispose and destructor
    void cleanup();

  public:
    // Methods
    double getWidth() override;
    double getHeight() override;
    double getChannels() override;
    std::shared_ptr<ArrayBuffer> getData() override;

    // Get the native bitmap handle
    HFImageBitmap getNativeHandle() const { return _bitmap; }

  private:
    HFImageBitmap _bitmap;
  };
} // namespace margelo::nitro::nitroinspireface
