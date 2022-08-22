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

#ifndef ENGINE_INCLUDE_HEXGON_GPU_COMMAND_BUFFER_HPP_
#define ENGINE_INCLUDE_HEXGON_GPU_COMMAND_BUFFER_HPP_

#include <Hexgon/Macro.hpp>
#include <cstdint>
#include <glm/glm.hpp>

namespace hexgon {
namespace gpu {

class Pipeline;
class VertexBuffer;
class IndexBuffer;

class HEX_API CommandBuffer {
 public:
  virtual ~CommandBuffer() = default;

  virtual void BindPipeline(Pipeline* pipeline) = 0;

  virtual void BindVertexBuffer(VertexBuffer* buffer, uint32_t slot) = 0;

  virtual void BindIndexBuffer(IndexBuffer* buffer) = 0;

  virtual void Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) = 0;

  virtual void DrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
                           uint32_t first_instance) = 0;

  virtual void SetSicssorBox(uint32_t offset_x, uint32_t offset_y, uint32_t wdith, uint32_t height) = 0;

  virtual glm::ivec4 CurrentScissorBox() = 0;
};

}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GPU_COMMAND_BUFFER_HPP_