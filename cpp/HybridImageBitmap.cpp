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

    // Create a copy of the data in an ArrayBuffer
    return ArrayBuffer::copy(bitmapData.data, dataSize);
  }

  // void HybridImageBitmap::writeToFile(const std::string &filePath)
  // {
  //   if (_bitmap == nullptr)
  //   {
  //     throw std::runtime_error("HybridImageBitmap is not initialized");
  //   }

  //   HResult result = HFImageBitmapWriteToFile(_bitmap, filePath.c_str());
  //   if (result != HSUCCEED)
  //   {
  //     throw std::runtime_error("Failed to write bitmap to file with error code: " + std::to_string(result));
  //   }
  // }

  void HybridImageBitmap::drawRect(const FaceRect &rect, const Color &color, double thickness)
  {
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
} // namespace margelo::nitro::nitroinspireface
