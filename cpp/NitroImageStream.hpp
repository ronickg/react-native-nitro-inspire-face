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
    NitroImageStream();

    // Constructor with stream
    NitroImageStream(HFImageStream stream);

    // Destructor
    ~NitroImageStream() override;

    // Override dispose to clean up resources
    void dispose() override;

  private:
    // Private cleanup method used by both dispose and destructor
    void cleanup();

  public:
    // Methods
    void writeImageToFile(const std::string &filePath) override;

    // Get the native stream handle
    HFImageStream getNativeHandle() const { return _stream; }

  private:
    HFImageStream _stream;
  };

} // namespace margelo::nitro::nitroinspireface
