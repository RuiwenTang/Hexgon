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

#include "GPU/VK/Buffer.hpp"

#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

void VertexBuffer::UploadData(void* data, size_t size) {
  if (size > m_buffer_size) {
    CleanUp();

    m_buffer_size = size;

    InitBuffer();
  }

  UploadDataInternal(data, size);
}

void VertexBuffer::CleanUp() {
  if (m_vk_buffer == VK_NULL_HANDLE) {
    return;
  }

  vmaDestroyBuffer(m_vma_allocator, m_vk_buffer, m_vma_allocation);

  m_vk_buffer = VK_NULL_HANDLE;
  m_vma_allocation = nullptr;
}

void VertexBuffer::InitBuffer() {
  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_info.size = m_buffer_size;
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vma_info{};
  vma_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  vma_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  if (vmaCreateBuffer(m_vma_allocator, &buffer_info, &vma_info, &m_vk_buffer, &m_vma_allocation, &m_vma_info) !=
      VK_SUCCESS) {
    HEX_CORE_ERROR("Failed allocate vertex buffer with size: {}", m_buffer_size);
  }
}

void VertexBuffer::UploadDataInternal(void* data, size_t size) {
  if (m_vma_info.pMappedData) {
    std::memcpy(m_vma_info.pMappedData, data, size);
    return;
  }

  void* vma_buffer_pointer = nullptr;
  if (vmaMapMemory(m_vma_allocator, m_vma_allocation, &vma_buffer_pointer) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed map vulkan memory to local");
    return;
  }

  std::memcpy(vma_buffer_pointer, data, size);

  vmaUnmapMemory(m_vma_allocator, m_vma_allocation);
}

}  // namespace hexgon::gpu::vk