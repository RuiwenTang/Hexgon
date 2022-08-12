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

#include "GPU/VK/FrameData.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

void VMABuffer::InitVMABuffer(VkBufferCreateInfo const& buffer_info, VmaAllocationCreateInfo const& vma_info) {
  if (vmaCreateBuffer(m_vma_allocator, &buffer_info, &vma_info, &m_vk_buffer, &m_vma_allocation, &m_vma_info) !=
      VK_SUCCESS) {
    HEX_CORE_ERROR("Failed allocate vertex buffer with size: {}", m_buffer_size);
  }

  m_buffer_size = buffer_info.size;
}

void VMABuffer::UploadDataToBuffer(void* data, size_t size, size_t offset) {
  if (!m_vma_info.pMappedData) {
    HEX_CORE_ERROR("vma buffer not mapped!!");
    return;
  }

  uint8_t* p = static_cast<uint8_t*>(m_vma_info.pMappedData) + offset;

  std::memcpy(p, data, size);
}

void VMABuffer::CleanUp() {
  if (m_vk_buffer == VK_NULL_HANDLE) {
    return;
  }

  vmaDestroyBuffer(m_vma_allocator, m_vk_buffer, m_vma_allocation);

  m_vk_buffer = VK_NULL_HANDLE;
  m_vma_allocation = nullptr;
}

void VertexBuffer::UploadData(void* data, size_t size) {
  if (size != BufferSize()) {
    CleanUp();
    InitBuffer(size);
  }
  UploadDataToBuffer(data, size, 0);
}

void VertexBuffer::InitBuffer(size_t size) {
  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_info.size = size;
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

  VmaAllocationCreateInfo vma_info{};
  vma_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  vma_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
  vma_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  InitVMABuffer(buffer_info, vma_info);
}

void IndexBuffer::UploadData(void* data, size_t size) {
  if (size != BufferSize()) {
    CleanUp();
    InitBuffer(size);
  }
  UploadDataToBuffer(data, size, 0);
}

void IndexBuffer::InitBuffer(size_t size) {
  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_info.size = size;
  buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

  VmaAllocationCreateInfo vma_info{};
  vma_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  vma_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
  vma_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  InitVMABuffer(buffer_info, vma_info);
}

UniformBuffer::UniformBuffer(size_t size, FrameInfoProvider* provider, VmaAllocator allocator)
    : gpu::UniformBuffer(size), m_frame_provider(provider) {
  Init(allocator);
}

void UniformBuffer::UploadData(void* data, size_t size, size_t offset) {
  m_vk_buffers[m_frame_provider->CurrentFrame()]->UploadData(data, size, offset);
}

void UniformBuffer::Init(VmaAllocator allocator) {
  for (uint32_t i = 0; i < m_frame_provider->TotalFrameCount(); i++) {
    auto buffer = std::make_unique<VMAUniformBuffer>(allocator);
    buffer->Init(GetBufferSize());

    m_vk_buffers.emplace_back(std::move(buffer));
  }
}

UniformBuffer::VMAUniformBuffer::VMAUniformBuffer(VmaAllocator allocator) : VMABuffer(allocator) {}

void UniformBuffer::VMAUniformBuffer::Init(size_t size) {
  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_info.size = size;
  buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

  VmaAllocationCreateInfo vma_info{};
  vma_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
  vma_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
  vma_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  InitVMABuffer(buffer_info, vma_info);
}

void UniformBuffer::VMAUniformBuffer::UploadData(void* data, size_t size, size_t offset) {
  UploadDataToBuffer(data, size, offset);
}

}  // namespace hexgon::gpu::vk