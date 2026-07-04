#include "HybridImageStream.hpp"
#include "HybridImageBitmap.hpp"
#include <stdexcept>
#include <string>
#include <optional>

namespace margelo::nitro::nitroinspireface
{
  HybridImageStream::HybridImageStream() : HybridObject(TAG), _stream(nullptr) {}

  HybridImageStream::HybridImageStream(HFImageStream stream) : HybridObject(TAG), _stream(stream)
  {
  }

  void HybridImageStream::cleanup()
  {
    // Exclusive over withNativeHandle/methods: wait for any in-flight native
    // call using this stream (e.g. a session's executeFaceTrack) before freeing.
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream != nullptr)
    {
      HFReleaseImageStream(_stream);
      _stream = nullptr;
    }
    _backing = nullptr;
    _backingBytes = 0;
  }

  HybridImageStream::~HybridImageStream()
  {
    cleanup();
  }

  void HybridImageStream::dispose()
  {
    cleanup();
  }

  size_t HybridImageStream::getExternalMemorySize() noexcept
  {
    // Non-blocking: getExternalMemorySize runs on the JS thread during object
    // materialization; never stall it behind a long native call holding _mutex.
    std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
    if (!lock.owns_lock())
    {
      return 0;
    }
    return _stream != nullptr ? _backingBytes : 0;
  }

  void HybridImageStream::writeImageToFile(const std::string &filePath)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream == nullptr)
    {
      throw std::runtime_error("HybridImageStream is not initialized");
    }

    HResult result = HFDeBugImageStreamDecodeSave(_stream, filePath.c_str());
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to write image to file with error code: " + std::to_string(result));
    }
  }

  void HybridImageStream::setBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream == nullptr)
    {
      throw std::runtime_error("HybridImageStream is not initialized");
    }

    if (!buffer || buffer->size() == 0)
    {
      throw std::runtime_error("Invalid buffer data");
    }

    HResult result = HFImageStreamSetBuffer(
        _stream,
        reinterpret_cast<uint8_t *>(buffer->data()),
        static_cast<HInt32>(width),
        static_cast<HInt32>(height));
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set image stream buffer with error code: " + std::to_string(result));
    }

    // Retain the JS buffer: the stream references it directly and only reads it
    // later during executeFaceTrack, so it must outlive this call.
    _backing = buffer;
    _backingBytes = buffer->size();
  }

  void HybridImageStream::setFormat(ImageFormat format)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream == nullptr)
    {
      throw std::runtime_error("HybridImageStream is not initialized");
    }

    HFImageFormat nativeFormat;
    switch (format)
    {
    case ImageFormat::RGB:
      nativeFormat = HF_STREAM_RGB;
      break;
    case ImageFormat::BGR:
      nativeFormat = HF_STREAM_BGR;
      break;
    case ImageFormat::RGBA:
      nativeFormat = HF_STREAM_RGBA;
      break;
    case ImageFormat::BGRA:
      nativeFormat = HF_STREAM_BGRA;
      break;
    case ImageFormat::YUV_NV12:
      nativeFormat = HF_STREAM_YUV_NV12;
      break;
    case ImageFormat::YUV_NV21:
      nativeFormat = HF_STREAM_YUV_NV21;
      break;
    case ImageFormat::I420:
      nativeFormat = HF_STREAM_I420;
      break;
    case ImageFormat::GRAY:
      nativeFormat = HF_STREAM_GRAY;
      break;
    default:
      throw std::runtime_error("Unsupported image format");
    }

    HResult result = HFImageStreamSetFormat(_stream, nativeFormat);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set image format with error code: " + std::to_string(result));
    }
  }

  void HybridImageStream::setRotation(CameraRotation rotation)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream == nullptr)
    {
      throw std::runtime_error("HybridImageStream is not initialized");
    }

    HFRotation nativeRotation;
    switch (rotation)
    {
    case CameraRotation::ROTATION_0:
      nativeRotation = HF_CAMERA_ROTATION_0;
      break;
    case CameraRotation::ROTATION_90:
      nativeRotation = HF_CAMERA_ROTATION_90;
      break;
    case CameraRotation::ROTATION_180:
      nativeRotation = HF_CAMERA_ROTATION_180;
      break;
    case CameraRotation::ROTATION_270:
      nativeRotation = HF_CAMERA_ROTATION_270;
      break;
    default:
      throw std::runtime_error("Unsupported rotation value");
    }

    HResult result = HFImageStreamSetRotation(_stream, nativeRotation);
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to set image rotation with error code: " + std::to_string(result));
    }
  }

  std::shared_ptr<HybridImageBitmapSpec> HybridImageStream::createImageBitmap(std::optional<bool> isRotate, std::optional<double> scale)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stream == nullptr)
    {
      throw std::runtime_error("HybridImageStream is not initialized");
    }

    HFImageBitmap bitmap = nullptr;
    HResult result = HFCreateImageBitmapFromImageStreamProcess(
        _stream,
        &bitmap,
        isRotate.has_value() ? (isRotate.value() ? 1 : 0) : 0,
        scale.has_value() ? static_cast<float>(scale.value()) : 1.0f);

    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to create image bitmap with error code: " + std::to_string(result));
    }

    return std::make_shared<HybridImageBitmap>(bitmap);
  }

} // namespace margelo::nitro::nitroinspireface
