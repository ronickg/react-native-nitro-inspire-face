#include "NitroImageStream.hpp"
#include <stdexcept>
#include <string>

namespace margelo::nitro::nitroinspireface
{
  NitroImageStream::NitroImageStream() : HybridObject(TAG), _stream(nullptr) {}

  NitroImageStream::NitroImageStream(HFImageStream stream) : HybridObject(TAG), _stream(stream)
  {
  }

  void NitroImageStream::cleanup()
  {
    if (_stream != nullptr)
    {
      HFReleaseImageStream(_stream);
      _stream = nullptr;
    }
  }

  NitroImageStream::~NitroImageStream()
  {
    cleanup();
  }

  void NitroImageStream::dispose()
  {
    cleanup();
  }

  void NitroImageStream::writeImageToFile(const std::string &filePath)
  {
    if (_stream == nullptr)
    {
      throw std::runtime_error("ImageStream is not initialized");
    }

    HResult result = HFDeBugImageStreamDecodeSave(_stream, filePath.c_str());
    if (result != HSUCCEED)
    {
      throw std::runtime_error("Failed to write image to file with error code: " + std::to_string(result));
    }
  }

} // namespace margelo::nitro::nitroinspireface
