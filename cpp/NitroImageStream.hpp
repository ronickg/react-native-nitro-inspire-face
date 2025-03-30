#pragma once

#include "HybridNitroImageStreamSpec.hpp"
#include "inspireface.h"

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the ImageStream module
   */
  class NitroImageStream : public virtual HybridNitroImageStreamSpec
  {
  public:
    // Default constructor required for autolink
    NitroImageStream() : HybridObject(TAG), _stream(nullptr) {}

    // Constructor with stream
    NitroImageStream(HFImageStream stream) : HybridObject(TAG), _stream(stream) {}

    // Destructor
    ~NitroImageStream() override;

  private:
    static constexpr auto TAG = "NitroImageStream";

  public:
    // Methods
    void writeImageToFile(const std::string &filePath) override;

    // Get the native stream handle
    HFImageStream getNativeHandle() const { return _stream; }

  private:
    HFImageStream _stream;
  };

} // namespace margelo::nitro::nitroinspireface
