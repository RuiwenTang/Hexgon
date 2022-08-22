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

#ifndef ENGINE_SRC_GPU_VK_COMMAND_BUFFER_HPP_
#define ENGINE_SRC_GPU_VK_COMMAND_BUFFER_HPP_

#include <vulkan/vulkan.h>

#include <Hexgon/GPU/CommandBuffer.hpp>

namespace hexgon {
namespace gpu {
namespace vk {

class CommandBuffer : public gpu::CommandBuffer {
 public:
  CommandBuffer() = default;
  ~CommandBuffer() override = default;

  VkCommandBuffer GetCMD() const;

  void SetCMD(VkCommandBuffer cmd);

  void BindPipeline(gpu::Pipeline* pipeline) override;

  void BindVertexBuffer(gpu::VertexBuffer* buffer, uint32_t slot) override;

  void BindIndexBuffer(gpu::IndexBuffer* buffer) override;

  void Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) override;

  void DrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
                   uint32_t first_instance) override;

  void SetSicssorBox(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height) override;

  glm::ivec4 CurrentScissorBox() override;

  void SetCurrentScissorBox(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height);

 private:
  VkCommandBuffer m_vk_cmd = VK_NULL_HANDLE;
  VkOffset2D m_scissor_offset = {};
  VkExtent2D m_scissor_extend = {};
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_COMMAND_BUFFER_HPP_