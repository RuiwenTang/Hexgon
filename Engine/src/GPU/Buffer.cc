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

#include <Hexgon/GPU/Buffer.hpp>

namespace hexgon {

static uint32_t get_data_type_size(DataType type) {
  switch (type) {
    case DataType::Float:
      return 4;
    case DataType::Float2:
      return 4 * 2;
    case DataType::Float3:
      return 4 * 3;
    case DataType::Float4:
      return 4 * 4;
    case DataType::Mat3:
      return 4 * 3 * 3;
    case DataType::Mat4:
      return 4 * 4 * 4;
    case DataType::Int:
      return 4;
    case DataType::Int2:
      return 4 * 2;
    case DataType::Int3:
      return 4 * 3;
    case DataType::Int4:
      return 4 * 4;
    default:
      return 0;
  }
}

BufferElement::BufferElement(std::string name, DataType type, uint32_t offset)
    : m_name(name), m_type(type), m_offset(offset), m_size(get_data_type_size(type)) {}

std::string const& BufferElement::Name() const { return m_name; }

DataType BufferElement::Type() const { return m_type; }

uint32_t BufferElement::Size() const { return m_size; }

uint32_t BufferElement::Offset() const { return m_offset; }

BufferLayout::BufferLayout(std::vector<BufferElement> elements) : m_element(std::move(elements)) {
  m_stride = CalculateStride();
}

uint32_t BufferLayout::CalculateStride() const {
  uint32_t stride = 0;

  for (auto const& ele : m_element) {
    stride += ele.Size();
  }

  return stride;
}

}  // namespace hexgon
