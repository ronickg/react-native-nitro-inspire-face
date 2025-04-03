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
    if (_stream != nullptr)
    {
      HFReleaseImageStream(_stream);
      _stream = nullptr;
    }
  }

  HybridImageStream::~HybridImageStream()
  {
    cleanup();
  }

  void HybridImageStream::dispose()
  {
    cleanup();
  }

  void HybridImageStream::writeImageToFile(const std::string &filePath)
  {
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

  void HybridImageStream::setFormat(ImageFormat format)
  {
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
