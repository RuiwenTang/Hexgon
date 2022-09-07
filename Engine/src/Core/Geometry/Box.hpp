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

#ifndef ENGINE_SRC_CORE_GEOMETRY_BOX_HPP_
#define ENGINE_SRC_CORE_GEOMETRY_BOX_HPP_

#include <Hexgon/Core/Geometry.hpp>

namespace hexgon {

class Box : public Geometry {
 public:
  Box(float width, float height, float depth, uint32_t width_segments, uint32_t height_segments,
      uint32_t depth_segment);

  ~Box() override = default;

  gpu::BufferLayout const& GetBufferLayout() override { return m_layout; }

 protected:
  void OnBuild() override;

 private:
  float m_width;
  float m_height;
  float m_depth;
  uint32_t m_width_segments;
  uint32_t m_height_segments;
  uint32_t m_depth_segments;
  gpu::BufferLayout m_layout;
};

}  // namespace hexgon

#endif  // ENGINE_SRC_CORE_GEOMETRY_BOX_HPP_