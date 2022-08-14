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

#include <Hexgon/GPU/Texture.hpp>

#include "LogPrivate.hpp"

namespace hexgon {
namespace gpu {

void Texture::Resize(uint32_t width, uint32_t height) {
  OnResize(GetWidth(), GetHeight(), width, height);

  m_width = width;
  m_height = height;
}

void Texture::UploadData(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height, void *data) {
  if (offset_x + width > GetWidth()) {
    HEX_CORE_ERROR("Texture upload failed since offset_x: {} + width: {} > texture width: {}", offset_x, width,
                   GetWidth());
    return;
  }

  if (offset_y + height > GetHeight()) {
    HEX_CORE_ERROR("Texture upload failed since offset_y: {} + height: {} > texture height: {}", offset_y, height,
                   GetHeight());
    return;
  }

  OnUploadData(offset_x, offset_y, width, height, data);
}

}  // namespace gpu
}  // namespace hexgon
