/*
 *   Copyright (c) 2022 RuiwenTang
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

#ifndef ENGINE_INCLUDE_HEXGON_GPU_TEXTURE_HPP_
#define ENGINE_INCLUDE_HEXGON_GPU_TEXTURE_HPP_

#include <Hexgon/GPU/Formats.hpp>
#include <Hexgon/Macro.hpp>
#include <cstdint>

namespace hexgon {
namespace gpu {

class HEX_API Texture {
 public:
  Texture(PixelFormat format, TextureUsage usage, TextureType type) : m_format(format), m_usage(usage), m_type(type) {}
  virtual ~Texture() = default;

  void Resize(uint32_t width, uint32_t height);

  void UploadData(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height, void* data);

  PixelFormat GetFormat() const { return m_format; }

  TextureUsage GetUsage() const { return m_usage; }

  TextureType GetType() const { return m_type; }

  uint32_t GetWidth() const { return m_width; }

  uint32_t GetHeight() const { return m_height; }

 protected:
  virtual void OnResize(uint32_t old_w, uint32_t old_h, uint32_t new_w, uint32_t new_h) = 0;

  virtual void OnUploadData(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height, void* data) = 0;

 private:
  PixelFormat m_format;
  TextureUsage m_usage;
  TextureType m_type;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
};

}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GPU_TEXTURE_HPP_