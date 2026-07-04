#pragma once

#include "HybridImageStreamSpec.hpp"
#include "inspireface.h"
#include "ImageFormat.hpp"
#include "CameraRotation.hpp"
#include "HybridImageBitmapSpec.hpp"
#include <NitroModules/ArrayBuffer.hpp>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace margelo::nitro::nitroinspireface
{
  class HybridImageBitmap;

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
    void setBuffer(const std::shared_ptr<ArrayBuffer> &buffer, double width, double height) override;
    void setFormat(ImageFormat format) override;
    void setRotation(CameraRotation rotation) override;
    std::shared_ptr<HybridImageBitmapSpec> createImageBitmap(std::optional<bool> isRotate = std::nullopt, std::optional<double> scale = std::nullopt) override;

    // Memory pressure hint for the JS GC (bytes of the backing buffer, if any).
    size_t getExternalMemorySize() noexcept override;

    // Run `fn` with the native stream handle held under the stream's lock, so a
    // concurrent dispose() can't free the handle mid-call. Throws if disposed.
    // Consumers that pass the stream to a native call (e.g. HybridSession's
    // executeFaceTrack) MUST use this instead of a bare getNativeHandle() —
    // otherwise the handle is only valid for the pointer read, not the call.
    // Lock ordering: a session holds its own lock, then enters this one
    // (session -> stream); nothing ever locks stream -> session, so no deadlock.
    template <typename Fn>
    auto withNativeHandle(Fn &&fn)
    {
      std::lock_guard<std::mutex> lock(_mutex);
      if (_stream == nullptr)
      {
        throw std::runtime_error("HybridImageStream is not initialized");
      }
      return fn(_stream);
    }

    // Keep the memory the stream references alive for the stream's lifetime.
    // The InspireFace ImageStream is a lightweight view over the caller's buffer
    // (the header only documents that the *Bitmap* copies), so without this the
    // JS ArrayBuffer / source bitmap can be GC'd before executeFaceTrack decodes
    // the stream -> read of freed memory. `reportBytes` is what
    // getExternalMemorySize should attribute to this stream (pass 0 when the
    // backing object already reports its own external memory, e.g. a bitmap).
    void setBacking(std::shared_ptr<void> backing, size_t reportBytes)
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _backing = std::move(backing);
      _backingBytes = reportBytes;
    }

  private:
    HFImageStream _stream;
    std::shared_ptr<void> _backing;
    size_t _backingBytes = 0;
    mutable std::mutex _mutex;
  };

} // namespace margelo::nitro::nitroinspireface
