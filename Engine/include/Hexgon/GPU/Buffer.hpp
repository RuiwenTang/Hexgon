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

#ifndef ENGINE_INCLUDE_HEXGON_GPU_BUFFER_HPP_
#define ENGINE_INCLUDE_HEXGON_GPU_BUFFER_HPP_

#include <string>
#include <vector>

namespace hexgon {

namespace gpu {

enum class DataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
};

class BufferElement final {
 public:
  BufferElement(std::string name, DataType type, uint32_t offset);

  ~BufferElement() = default;

  std::string const& Name() const;

  DataType Type() const;

  uint32_t Size() const;

  uint32_t Offset() const;

 private:
  std::string m_name;
  DataType m_type;
  uint32_t m_offset;
  uint32_t m_size;
};

class BufferLayout final {
 public:
  BufferLayout(std::vector<BufferElement> elements);

  ~BufferLayout() = default;

  std::vector<BufferElement> const& Elements() const;

  uint32_t Stride() const;

 private:
  uint32_t CalculateStride() const;

 private:
  std::vector<BufferElement> m_element;
  uint32_t m_stride;
};

class VertexBuffer {
 public:
  VertexBuffer(BufferLayout const& layout) : m_layout(layout) {}
  virtual ~VertexBuffer() = default;

  BufferLayout const& Layout() const { return m_layout; }

  virtual void UploadData(void* data, size_t size) = 0;

 private:
  BufferLayout m_layout;
};

class IndexBuffer {
 public:
  IndexBuffer() = default;
  virtual ~IndexBuffer() = default;

  virtual void UploadData(void* data, size_t size) = 0;
};

}  // namespace gpu

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GPU_BUFFER_HPP_