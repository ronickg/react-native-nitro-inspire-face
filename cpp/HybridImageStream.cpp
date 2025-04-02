#include "HybridImageStream.hpp"
#include <stdexcept>
#include <string>

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

} // namespace margelo::nitro::nitroinspireface
