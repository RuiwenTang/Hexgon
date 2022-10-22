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

#include <Hexgon/IO/Image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "LogPrivate.hpp"

namespace hexgon::io {

std::shared_ptr<Image> Image::Load(std::string const& path) {
  int32_t width = 0;
  int32_t height = 0;
  int32_t channels = 0;
  void* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    return std::shared_ptr<Image>();
  }

  if (channels < 3) {
    HEX_CORE_ERROR("Currently Image Texture only support RGBA or RGB formats");
    return std::shared_ptr<Image>();
  }

  std::shared_ptr<Image> img{new Image};

  img->m_width = width;
  img->m_height = height;
  img->m_data = data;

  if (channels == 4) {
    img->m_format = gpu::PixelFormat::R8G8B8A8UNormInt;
  } else if (channels == 3) {
    img->m_format = gpu::PixelFormat::R8G8B8A8UNormInt;
  }

  return img;
}
std::shared_ptr<Image> Image::CreateEmpty(uint32_t width, uint32_t height) {
  std::shared_ptr<Image> img{new Image};

  img->m_width = width;
  img->m_height = height;
  img->m_data = STBI_MALLOC(width * height * 4);
  img->m_format = gpu::PixelFormat::R8G8B8A8UNormInt;

  return img;
}

void Image::PutPixel(uint32_t x, uint32_t y, uint32_t pixel) {
  if (x >= m_width || y >= m_height) {
    return;
  }

  uint32_t* p = static_cast<uint32_t*>(m_data);

  p[y * m_width + x] = pixel;
}

void Image::CleanUp() {
  if (m_data) {
    stbi_image_free(m_data);

    m_data = nullptr;
  }
}

}  // namespace hexgon::io