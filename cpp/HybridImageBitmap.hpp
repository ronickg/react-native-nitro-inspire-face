#pragma once

#include "HybridImageBitmapSpec.hpp"
#include "inspireface.h"
#include "FaceRect.hpp"
#include "Color.hpp"
#include "Point2f.hpp"
#include "Point2i.hpp"
#include <NitroModules/ArrayBuffer.hpp>
#include <NitroModules/NitroLogger.hpp>
#include <memory>
#include <string>

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the HybridImageBitmap module
   */
  class HybridImageBitmap : public virtual HybridImageBitmapSpec
  {
  public:
    // Default constructor required for autolink
    HybridImageBitmap();

    // Constructor with bitmap
    HybridImageBitmap(HFImageBitmap bitmap);

    // Destructor
    ~HybridImageBitmap() override;

    // Override dispose to clean up resources
    void dispose() override;

  private:
    // Private cleanup method used by both dispose and destructor
    void cleanup();

  public:
    // Properties
    double getWidth() override;
    double getHeight() override;
    double getChannels() override;
    std::shared_ptr<ArrayBuffer> getData() override;

    // Methods
    // void writeToFile(const std::string &filePath) override;
    void drawRect(const FaceRect &rect, const Color &color, double thickness) override;
    void drawCircleF(const Point2f &point, double radius, const Color &color, double thickness) override;
    void drawCircle(const Point2i &point, double radius, const Color &color, double thickness) override;

    // Get the native bitmap handle
    HFImageBitmap getNativeHandle() const { return _bitmap; }

  private:
    HFImageBitmap _bitmap;
  };
} // namespace margelo::nitro::nitroinspireface
