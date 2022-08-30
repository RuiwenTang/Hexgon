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

#include "Core/Geometry/Box.hpp"

#include <glm/glm.hpp>

namespace hexgon {

enum { X = 0, Y = 1, Z = 2 };
enum { WIDTH = 0, HEIGHT = 1, DEPTH = 2 };

static void build_plane(const glm::u32vec3& index, const glm::vec2& uv_dir, const glm::vec3& size,
                        const glm::u32vec2& grid, std::vector<float>& vertics, std::vector<uint32_t>& indices) {
  float segmentWidth = size[WIDTH] / grid.x;
  float segmentHeight = size[HEIGHT] / grid.y;

  float widthHalf = size[WIDTH] / 2.f;
  float heightHalf = size[HEIGHT] / 2.f;
  float depthHalf = size[DEPTH] / 2.f;

  int32_t gridX1 = grid.x + 1;
  int32_t gridY1 = grid.y + 1;

  uint32_t vertexStart = vertics.size() / 8;
  uint32_t vertexCounter = 0;
  for (int32_t iy = 0; iy < gridY1; iy++) {
    float y = iy * segmentHeight - heightHalf;
    for (int32_t ix = 0; ix < gridX1; ix++) {
      float x = ix * segmentWidth - widthHalf;
      glm::vec3 vertex = {};
      vertex[index.x] = x * uv_dir.x;
      vertex[index.y] = y * uv_dir.y;
      vertex[index.z] = depthHalf;
      // vertex
      vertics.emplace_back(vertex.x);
      vertics.emplace_back(vertex.y);
      vertics.emplace_back(vertex.z);
      // normal
      glm::vec3 normal = {};
      normal[index.x] = 0;
      normal[index.y] = 0;
      normal[index.z] = size[DEPTH] > 0 ? 1.f : -1.f;

      vertics.emplace_back(normal.x);
      vertics.emplace_back(normal.y);
      vertics.emplace_back(normal.z);

      // uvs
      glm::vec2 uv{ix / grid.x, 1 - (iy / grid.y)};

      vertics.emplace_back(uv.x);
      vertics.emplace_back(uv.y);

      vertexCounter++;
    }
  }

  // indices
  for (int32_t iy = 0; iy < grid.y; iy++) {
    for (int32_t ix = 0; ix < grid.x; ix++) {
      uint32_t a = vertexStart + ix + gridX1 * iy;
      uint32_t b = vertexStart + ix + gridX1 * (iy + 1);
      uint32_t c = vertexStart + (ix + 1) + gridX1 * (iy + 1);
      uint32_t d = vertexStart + (ix + 1) + gridX1 * iy;

      indices.emplace_back(a);
      indices.emplace_back(b);
      indices.emplace_back(d);

      indices.emplace_back(b);
      indices.emplace_back(c);
      indices.emplace_back(d);
    }
  }
}

Box::Box(float width, float height, float depth, uint32_t width_segments, uint32_t height_segments,
         uint32_t depth_segment)
    : m_width(width),
      m_height(height),
      m_depth(depth),
      m_width_segments(width_segments),
      m_height_segments(height_segments),
      m_depth_segments(depth_segment),
      m_layout({
          gpu::BufferElement{"pos", gpu::DataType::Float3, 0},
          gpu::BufferElement{"normal", gpu::DataType::Float3, sizeof(float) * 3},
          gpu::BufferElement{"uv", gpu::DataType::Float2, sizeof(float) * 6},
      }) {}

void Box::OnBuild() {
  build_plane(glm::u32vec3{Z, Y, X}, glm::vec2{-1.f, -1.f}, glm::vec3{m_depth, m_height, m_width},
              glm::u32vec2{m_depth_segments, m_height_segments}, GetCurrentVertex(), GetCurrentIndex());

  build_plane(glm::u32vec3{Z, Y, X}, glm::vec2{1.f, -1.f}, glm::vec3{m_depth, m_height, -m_width},
              glm::u32vec2{m_depth_segments, m_height_segments}, GetCurrentVertex(), GetCurrentIndex());

  build_plane(glm::u32vec3{X, Z, Y}, glm::vec2{1.f, 1.f}, glm::vec3{m_width, m_depth, m_height},
              glm::u32vec2{m_width_segments, m_depth_segments}, GetCurrentVertex(), GetCurrentIndex());

  build_plane(glm::u32vec3{X, Z, Y}, glm::vec2{1.f, -1.f}, glm::vec3{m_width, m_depth, -m_height},
              glm::u32vec2{m_width_segments, m_depth_segments}, GetCurrentVertex(), GetCurrentIndex());

  build_plane(glm::u32vec3{X, Y, Z}, glm::vec2{1.f, -1.f}, glm::vec3{m_width, m_height, m_depth},
              glm::u32vec2{m_depth_segments, m_height_segments}, GetCurrentVertex(), GetCurrentIndex());

  build_plane(glm::u32vec3{X, Y, Z}, glm::vec2{-1.f, -1.f}, glm::vec3{m_width, m_height, -m_depth},
              glm::u32vec2{m_width_segments, m_height_segments}, GetCurrentVertex(), GetCurrentIndex());
}

std::shared_ptr<Geometry> Geometry::MakeBox(float width, float height, float depth, uint32_t width_segments,
                                            uint32_t height_segments, uint32_t depth_segments) {
  return std::make_shared<Box>(width, height, depth, width_segments, height_segments, depth_segments);
}

}  // namespace hexgon
