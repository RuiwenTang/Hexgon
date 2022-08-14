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

#ifndef ENGINE_SRC_GPU_VK_TEXTURE_HPP_
#define ENGINE_SRC_GPU_VK_TEXTURE_HPP_

#include <vk_mem_alloc.h>

#include <Hexgon/GPU/Texture.hpp>

namespace hexgon {
namespace gpu {
namespace vk {

class GraphicsContext;

class Texture : public gpu::Texture {
 public:
  Texture(PixelFormat format, TextureUsage usage, TextureType type, GraphicsContext* ctx, VmaAllocator allocator);

  ~Texture() override { CleanUp(); }

  void Init();

  void CleanUp();

  void PrepareForDraw();

  VkSampler Get2dSampler() const { return m_2d_sampler; }

  VkImageView GetImageView() const { return m_image_view; }

  VkImageLayout GetImageLayout() const { return m_image_layout; }

 protected:
  void OnResize(uint32_t old_w, uint32_t old_h, uint32_t new_w, uint32_t new_h) override;

  void OnUploadData(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height, void* data) override;

  void InitImage(uint32_t width, uint32_t height);

  void TransforImageLayout(VkCommandBuffer cmd, VkImageLayout new_layout);

  void CopyBufferToImage(VkCommandBuffer cmd, VkBuffer vk_buffer, VkBufferImageCopy const& copy_info);

 private:
  GraphicsContext* m_ctx;
  VmaAllocator m_allocator;
  VmaAllocation m_allocation_info = {};
  VkImageUsageFlags m_vk_usage = {};
  VkFormat m_vk_format = {};
  VkSampler m_2d_sampler = {};
  VkImageSubresourceRange m_sub_range = {};
  VkImage m_image = {};
  VkImageLayout m_image_layout = {};
  VkImageView m_image_view = {};
  VkExtent3D m_image_extent = {};
};

}  // namespace vk
}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_SRC_GPU_VK_TEXTURE_HPP_