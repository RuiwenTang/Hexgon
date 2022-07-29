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

#ifndef ENGINE_SRC_GPU_VK_BUFFER_HPP_
#define ENGINE_SRC_GPU_VK_BUFFER_HPP_

#include <vk_mem_alloc.h>

#include <Hexgon/GPU/Buffer.hpp>

namespace hexgon {
namespace gpu {
namespace vk {

class VertexBuffer : public gpu::VertexBuffer {
 public:
  VertexBuffer(BufferLayout layout, VmaAllocator allocator)
      : gpu::VertexBuffer(layout),
        m_vma_allocator(allocator),
        m_buffer_size(0),
        m_vk_buffer(VK_NULL_HANDLE),
        m_vma_allocation(nullptr),
        m_vma_info() {}

  ~VertexBuffer() override { CleanUp(); }

  void UploadData(void* data, size_t size) override;

 private:
  void CleanUp();
  void InitBuffer();
  void UploadDataInternal(void* data, size_t size);

 private:
  VkDevice m_vk_device;
  VmaAllocator m_vma_allocator;
  size_t m_buffer_size;
  VkBuffer m_vk_buffer;
  VmaAllocation m_vma_allocation;
  VmaAllocationInfo m_vma_info;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_BUFFER_HPP_