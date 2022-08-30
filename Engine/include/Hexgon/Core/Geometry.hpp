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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_GEOMETRY_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_GEOMETRY_HPP_

#include <Hexgon/GPU/Buffer.hpp>
#include <Hexgon/Macro.hpp>
#include <memory>
#include <vector>

namespace hexgon {

class GraphicsContext;

class HEX_API Geometry {
 public:
  Geometry() = default;
  virtual ~Geometry() = default;

  void Build();

  void InitBuffer(GraphicsContext* ctx);

  virtual gpu::BufferLayout const& GetBufferLayout() = 0;

  size_t GetIndexCount() const { return m_index.size(); }

  static std::shared_ptr<Geometry> MakeBox(float width = 1.f, float height = 1.f, float depth = 1.f,
                                           uint32_t width_segments = 1, uint32_t height_segments = 1,
                                           uint32_t depth_segment = 1);

 protected:
  virtual void OnBuild() = 0;

  std::vector<float>& GetCurrentVertex() { return m_vertex; }

  std::vector<uint32_t>& GetCurrentIndex() { return m_index; }

 private:
  size_t GetIndexDataSize() const { return m_index.size() * sizeof(uint32_t); }

  size_t GetVertexDataSize() const { return m_vertex.size() * sizeof(float); }

 private:
  std::vector<float> m_vertex;
  std::vector<uint32_t> m_index;

  std::unique_ptr<gpu::VertexBuffer> m_gpu_vertex = {};
  std::unique_ptr<gpu::IndexBuffer> m_gpu_index = {};
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_GEOMETRY_HPP_
