#pragma once

#include "HybridImageStreamSpec.hpp"
#include "inspireface.h"

namespace margelo::nitro::nitroinspireface
{
  /**
   * Implementation of the HybridImageStream module
   */
  class HybridImageStream : public virtual HybridImageStreamSpec
  {
  public:
    // Default constructor required for autolink
    HybridImageStream();

    // Constructor with stream
    HybridImageStream(HFImageStream stream);

    // Destructor
    ~HybridImageStream() override;

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
