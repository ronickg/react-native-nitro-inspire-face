#include "NitroImageBitmap.hpp"
#include <stdexcept>
#include <string>

namespace margelo::nitro::nitroinspireface
{
  NitroImageBitmap::NitroImageBitmap() : HybridObject(TAG), _bitmap(nullptr) {}

  NitroImageBitmap::NitroImageBitmap(HFImageBitmap bitmap) : HybridObject(TAG), _bitmap(bitmap)
  {
  }

  void NitroImageBitmap::cleanup()
  {
    if (_bitmap != nullptr)
    {
      HFReleaseImageBitmap(_bitmap);
      _bitmap = nullptr;
    }
  }

  NitroImageBitmap::~NitroImageBitmap()
  {
    cleanup();
  }

  void NitroImageBitmap::dispose()
  {
    cleanup();
  }

  double NitroImageBitmap::getWidth()
  {
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("ImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.width);
  }

  double NitroImageBitmap::getHeight()
  {
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("ImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.height);
  }

  double NitroImageBitmap::getChannels()
  {
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("ImageBitmap is not initialized");
    }

    HFImageBitmapData bitmapData{};
    HResult result = HFImageBitmapGetData(_bitmap, &bitmapData);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to get bitmap data with error code: " + std::to_string(result));
    }

    return static_cast<double>(bitmapData.channels);
  }

  std::shared_ptr<ArrayBuffer> NitroImageBitmap::getData()
  {
    if (_bitmap == nullptr)
    {
      throw std::runtime_error("ImageBitmap is not initialized");
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
} // namespace margelo::nitro::nitroinspireface
