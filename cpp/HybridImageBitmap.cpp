#include "HybridImageBitmap.hpp"
#include <stdexcept>
#include <string>

namespace margelo::nitro::nitroinspireface
{
  HybridImageBitmap::HybridImageBitmap() : HybridObject(TAG), _bitmap(nullptr) {}

  HybridImageBitmap::HybridImageBitmap(HFImageBitmap bitmap) : HybridObject(TAG), _bitmap(bitmap)
  {
  }

  void HybridImageBitmap::cleanup()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap != nullptr)
    {
      HFReleaseImageBitmap(_bitmap);
      _bitmap = nullptr;
    }
  }

  HybridImageBitmap::~HybridImageBitmap()
  {
    cleanup();
  }

  void HybridImageBitmap::dispose()
  {
    cleanup();
  }

  double HybridImageBitmap::getWidth()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.width);
  }

  double HybridImageBitmap::getHeight()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.height);
  }

  double HybridImageBitmap::getChannels()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.channels);
  }

  std::shared_ptr<ArrayBuffer> HybridImageBitmap::getData()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    // Calculate total size of the image data
    size_t dataSize = static_cast<size_t>(bitmapData.width) * static_cast<size_t>(bitmapData.height) * static_cast<size_t>(bitmapData.channels);

    // Zero-copy: wrap the bitmap's internal buffer directly. The deleter
    // captures a strong reference to this HybridImageBitmap so the buffer
    // genuinely pins the bitmap against GC for as long as JS (on any runtime)
    // holds the ArrayBuffer. Note this cannot protect against an explicit
    // dispose() while the buffer is still referenced — copy the data out
    // before disposing the bitmap.
    auto self = shared_from_this();
    return std::make_shared<NativeArrayBuffer>(
        bitmapData.data,
        dataSize,
        [self]()
        {
          // The captured shared_ptr keeps the bitmap (and its buffer) alive
          // until the ArrayBuffer is destroyed; releasing it here lets the
          // bitmap free naturally.
        });
  }

  void HybridImageBitmap::drawRect(const FaceRect &rect, const Color &color, double thickness)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFaceRect nativeRect = {
        static_cast<HInt32>(rect.x),
        static_cast<HInt32>(rect.y),
        static_cast<HInt32>(rect.width),
        static_cast<HInt32>(rect.height)};

    HColor nativeColor = {
        static_cast<HFloat>(color.r),
        static_cast<HFloat>(color.g),
        static_cast<HFloat>(color.b)};

    HResult result = HFImageBitmapDrawRect(_bitmap, nativeRect, nativeColor, static_cast<int>(thickness));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to draw rectangle with error code: " + std::to_string(result));
    }
  }

  void HybridImageBitmap::drawCircleF(const Point2f &point, double radius, const Color &color, double thickness)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HPoint2f nativePoint = {
        static_cast<HFloat>(point.x),
        static_cast<HFloat>(point.y)};

    HColor nativeColor = {
        static_cast<HFloat>(color.r),
        static_cast<HFloat>(color.g),
        static_cast<HFloat>(color.b)};

    HResult result = HFImageBitmapDrawCircleF(_bitmap, nativePoint, static_cast<int>(radius), nativeColor, static_cast<int>(thickness));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to draw circle with error code: " + std::to_string(result));
    }
  }

  void HybridImageBitmap::drawCircle(const Point2i &point, double radius, const Color &color, double thickness)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HPoint2i nativePoint = {
        static_cast<HInt32>(point.x),
        static_cast<HInt32>(point.y)};

    HColor nativeColor = {
        static_cast<HFloat>(color.r),
        static_cast<HFloat>(color.g),
        static_cast<HFloat>(color.b)};

    HResult result = HFImageBitmapDrawCircle(_bitmap, nativePoint, static_cast<int>(radius), nativeColor, static_cast<int>(thickness));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to draw circle with error code: " + std::to_string(result));
    }
  }
  void HybridImageBitmap::writeToFile(const std::string &filePath)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HResult result = HFImageBitmapWriteToFile(_bitmap, filePath.c_str());
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to write bitmap to file with error code: " + std::to_string(result));
    }
  }

  std::shared_ptr<HybridImageBitmapSpec> HybridImageBitmap::copy()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("HybridImageBitmap is not initialized");
    }

    HFImageBitmap copyHandle = nullptr;
    HResult result = HFImageBitmapCopy(_bitmap, &copyHandle);
    if (result != HSUCCEED || copyHandle == nullptr)
    {
      throw std::runtime_error("Failed to copy bitmap with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(copyHandle);
  }

  size_t HybridImageBitmap::getExternalMemorySize() noexcept
  {
    // Report the pixel buffer size so the JS GC feels the real weight of a
    // leaked bitmap instead of a few-byte wrapper. Non-blocking: runs on the JS
    // thread during object materialization, so never stall behind a native call.
    std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
    if (!lock.owns_lock() || _bitmap == nullptr)
    {
      return 0;
    }
    HFImageBitmapData bitmapData{};
    if (HFImageBitmapGetData(_bitmap, &bitmapData) != HSUCCEED)
    {
      return 0;
    }
    return static_cast<size_t>(bitmapData.width) * static_cast<size_t>(bitmapData.height) * static_cast<size_t>(bitmapData.channels);
  }

} // namespace margelo::nitro::nitroinspireface
