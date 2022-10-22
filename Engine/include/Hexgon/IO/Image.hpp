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

#ifndef ENGINE_INCLUDE_HEXGON_IO_IMAGE_HPP_
#define ENGINE_INCLUDE_HEXGON_IO_IMAGE_HPP_

#include <Hexgon/GPU/Formats.hpp>
#include <Hexgon/Macro.hpp>
#include <memory>
#include <string>

namespace hexgon {
namespace io {
class HEX_API Image {
 public:
  ~Image() { CleanUp(); }

  static std::shared_ptr<Image> Load(std::string const& path);
  static std::shared_ptr<Image> CreateEmpty(uint32_t width, uint32_t height);

  uint32_t GetWidth() const { return m_width; }
  uint32_t GetHeight() const { return m_height; }

  gpu::PixelFormat GetFormat() const { return m_format; }

  void* GetRawData() const { return m_data; }

  void PutPixel(uint32_t x, uint32_t y, uint32_t pixel);

 private:
  Image() = default;

  void CleanUp();

 private:
  void* m_data = nullptr;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
  gpu::PixelFormat m_format = gpu::PixelFormat::Unknown;
};

}  // namespace io
}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_IO_IMAGE_HPP_
