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

#include "GPU/VK/CommandBuffer.hpp"

#include "GPU/VK/Buffer.hpp"
#include "GPU/VK/Pipeline.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

VkCommandBuffer CommandBuffer::GetCMD() const { return m_vk_cmd; }

void CommandBuffer::SetCMD(VkCommandBuffer cmd) { m_vk_cmd = cmd; }

void CommandBuffer::BindPipeline(gpu::Pipeline *pipeline) {
  auto vk_pipeline = dynamic_cast<vk::Pipeline *>(pipeline);

  if (vk_pipeline == nullptr) {
    HEX_CORE_ERROR("pipeline 0x{:x} is not a Vulkan pipeline", reinterpret_cast<intptr_t>(pipeline));
    return;
  }

  vkCmdBindPipeline(m_vk_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline->NativePipeline());
}

void CommandBuffer::BindVertexBuffer(gpu::VertexBuffer *buffer, uint32_t slot) {
  auto vk_buffer = dynamic_cast<vk::VertexBuffer *>(buffer);

  if (vk_buffer == nullptr) {
    HEX_CORE_ERROR("buffer 0x{:x} is not a vulkan vertex buffer", reinterpret_cast<intptr_t>(buffer));
    return;
  }

  VkBuffer native_buffer = vk_buffer->NativeBuffer();
  VkDeviceSize offset = 0;
  vkCmdBindVertexBuffers(m_vk_cmd, slot, 1, &native_buffer, &offset);
}

void CommandBuffer::BindIndexBuffer(gpu::IndexBuffer *buffer) {
  auto vk_buffer = dynamic_cast<vk::IndexBuffer *>(buffer);

  if (vk_buffer == nullptr) {
    HEX_CORE_ERROR("buffer 0x{:x} is not a vulkan index buffer", reinterpret_cast<intptr_t>(buffer));
    return;
  }

  vkCmdBindIndexBuffer(m_vk_cmd, vk_buffer->NativeBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void CommandBuffer::Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex,
                         uint32_t first_instance) {
  vkCmdDraw(m_vk_cmd, vertex_count, instance_count, first_vertex, first_instance);
}

void CommandBuffer::DrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
                                uint32_t first_instance) {
  vkCmdDrawIndexed(m_vk_cmd, index_count, instance_count, first_index, 0, first_instance);
}

void CommandBuffer::SetSicssorBox(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height) {
  m_scissor_offset.x = offset_x;
  m_scissor_offset.y = offset_y;

  m_scissor_extend.width = width;
  m_scissor_extend.height = height;

  VkRect2D scissor{};

  scissor.offset = m_scissor_offset;
  scissor.extent = m_scissor_extend;

  vkCmdSetScissor(m_vk_cmd, 0, 1, &scissor);
}

glm::ivec4 CommandBuffer::CurrentScissorBox() {
  return glm::ivec4{m_scissor_offset.x, m_scissor_offset.y, m_scissor_extend.width, m_scissor_extend.height};
}

void CommandBuffer::SetCurrentScissorBox(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height) {
  m_scissor_offset.x = offset_x;
  m_scissor_offset.y = offset_y;

  m_scissor_extend.width = width;
  m_scissor_extend.height = height;
}

}  // namespace hexgon::gpu::vk