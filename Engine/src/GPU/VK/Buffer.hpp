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
#include <vector>

namespace hexgon {
namespace gpu {
namespace vk {

class VMABuffer {
 public:
  VMABuffer(VmaAllocator allocator)
      : m_vma_allocator(allocator), m_vma_info(), m_vma_allocation(), m_buffer_size(0), m_vk_buffer(VK_NULL_HANDLE) {}

  virtual ~VMABuffer() { CleanUp(); }

  VkBuffer NativeBuffer() const { return m_vk_buffer; }

  size_t BufferSize() const { return m_buffer_size; }

 protected:
  void InitVMABuffer(VkBufferCreateInfo const& buffer_info, VmaAllocationCreateInfo const& vma_info);

  void UploadDataToBuffer(void* data, size_t size, size_t offset);

  void CleanUp();

 private:
  VmaAllocator m_vma_allocator;
  VmaAllocationInfo m_vma_info;
  VmaAllocation m_vma_allocation;
  size_t m_buffer_size;
  VkBuffer m_vk_buffer;
};

class VertexBuffer : public gpu::VertexBuffer, public VMABuffer {
 public:
  VertexBuffer(BufferLayout layout, VmaAllocator allocator) : gpu::VertexBuffer(layout), VMABuffer(allocator) {}

  ~VertexBuffer() override = default;

  void UploadData(void* data, size_t size) override;

 private:
  void InitBuffer(size_t size);
};

class IndexBuffer : public gpu::IndexBuffer, public VMABuffer {
 public:
  IndexBuffer(VmaAllocator allocator) : gpu::IndexBuffer(), VMABuffer(allocator) {}

  ~IndexBuffer() override = default;

  void UploadData(void* data, size_t size) override;

 private:
  void InitBuffer(size_t size);
};

class FrameInfoProvider;

class UniformBuffer : public gpu::UniformBuffer {
  class VMAUniformBuffer : public VMABuffer {
   public:
    VMAUniformBuffer(VmaAllocator allocator);

    ~VMAUniformBuffer() override = default;

    void Init(size_t size);

    void UploadData(void* data, size_t size, size_t offset);
  };

 public:
  UniformBuffer(size_t size, FrameInfoProvider* provider, VmaAllocator allocator);

  ~UniformBuffer() override = default;

  void UploadData(void* data, size_t size, size_t offset) override;

 private:
  void Init(VmaAllocator allocator);

 private:
  FrameInfoProvider* m_frame_provider;
  std::vector<VMAUniformBuffer> m_vk_buffers;
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_BUFFER_HPP_