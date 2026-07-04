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
#include <mutex>
#include <stdexcept>
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
    void drawRect(const FaceRect &rect, const Color &color, double thickness) override;
    void drawCircleF(const Point2f &point, double radius, const Color &color, double thickness) override;
    void drawCircle(const Point2i &point, double radius, const Color &color, double thickness) override;
    void writeToFile(const std::string &filePath) override;
    std::shared_ptr<HybridImageBitmapSpec> copy() override;

    // Memory pressure hint for the JS GC (pixel buffer size).
    size_t getExternalMemorySize() noexcept override;

    // Run `fn` with the native bitmap handle held under the bitmap's lock, so a
    // concurrent dispose() can't free the handle mid-call. Throws if disposed.
    // Consumers passing the bitmap to a native call (createImageStreamFromBitmap)
    // MUST use this instead of a bare getNativeHandle().
    template <typename Fn>
    auto withNativeHandle(Fn &&fn)
    {
      std::lock_guard<std::mutex> lock(_mutex);
      if (_bitmap == nullptr)
      {
        throw std::runtime_error("HybridImageBitmap is not initialized");
      }
      return fn(_bitmap);
    }

  private:
    HFImageBitmap _bitmap;
    mutable std::mutex _mutex;
  };
} // namespace margelo::nitro::nitroinspireface
