/*
 *   Copyright (c) 2023 RuiwenTang
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#pragma once

#include <Hexgon/Render/Type.hpp>
#include <cstddef>
#include <cstdint>
#include <string>

namespace hexgon {

enum class TextureType {
  k1D,
  k2D,
  k3D,
};

enum class PixelFormat {
  kUnknown,
  kA8UNorm,
  kR8G8B8A8Unorm,
  kB8G8R8A8Unorm,
};

using TextureUsageMask = uint32_t;

enum TextureUsage : TextureUsageMask {
  kUnknown = 0,
  kShaderRead = 1 << 0,
  kShaderWrite = 1 << 1,
  kRenderTarget = 1 << 2,
  kCopySrc = 1 << 3,
  kCopyDst = 1 << 4,
};

struct TextureDescriptor {
  std::string label = "Texture";
  PixelFormat format = PixelFormat::kR8G8B8A8Unorm;
  TextureType type = TextureType::k2D;
  TextureUsageMask usage = TextureUsage::kShaderRead | TextureUsage::kCopyDst;

  StorageMode storage = StorageMode::kHostVisible;

  uint32_t width = 0;
  uint32_t height = 0;
};

struct TextureRange {
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t width = 0;
  uint32_t height = 0;
};

class Texture {
 public:
  Texture(TextureDescriptor desc) : m_desc(desc) {}

  virtual ~Texture() = default;

  TextureType GetTextureType() const { return m_desc.type; }

  TextureUsageMask GetUsage() const { return m_desc.usage; }

  PixelFormat GetFormat() const { return m_desc.format; }

  uint32_t GetWidth() const { return m_desc.width; }

  uint32_t GetHeight() const { return m_desc.height; }

  void UploadData(void* data, size_t len, const TextureRange& range);

 protected:
  virtual void OnUploadData(void* data, size_t len, const TextureRange& range) = 0;

 private:
  TextureDescriptor m_desc;
};

}  // namespace hexgon
