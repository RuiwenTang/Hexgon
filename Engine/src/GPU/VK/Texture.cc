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

#include "GPU/VK/Texture.hpp"

#include <memory>

#include "GPU/VK/Buffer.hpp"
#include "GPU/VK/Formats.hpp"
#include "GPU/VK/GraphicsContext.hpp"
#include "LogPrivate.hpp"

namespace hexgon::gpu::vk {

static VkImageAspectFlags texture_type_to_aspect(TextureType type) {
  switch (type) {
    case TextureType::kImageTexture:
      return VK_IMAGE_ASPECT_COLOR_BIT;
    case TextureType::kDepthTexture:
      return VK_IMAGE_ASPECT_DEPTH_BIT;
    default:
      return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  }
}

static uint32_t vk_format_comp(VkFormat format) {
  if (format == VK_FORMAT_R8G8B8_UNORM) {
    return 3;
  } else if (format == VK_FORMAT_R8G8B8A8_UNORM) {
    return 4;
  } else if (format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
    return 8;
  } else if (format == VK_FORMAT_D24_UNORM_S8_UINT) {
    return 4;
  } else if (format == VK_FORMAT_D16_UNORM_S8_UINT) {
    return 3;
  } else {
    return 1;
  }
}

Texture::Texture(PixelFormat format, TextureUsage usage, TextureType type, GraphicsContext* ctx, VmaAllocator allocator)
    : gpu::Texture(format, usage, type), m_ctx(ctx), m_allocator(allocator) {
  if (usage == TextureUsage::ShaderRead && type == TextureType::kImageTexture) {
    m_vk_usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  }
}

void Texture::Init() {
  m_vk_format = Convertor<PixelFormat, VkFormat>::ToVulkan(GetFormat());

  m_sub_range.aspectMask = texture_type_to_aspect(GetType());
  m_sub_range.baseMipLevel = 0;
  m_sub_range.levelCount = 1;
  m_sub_range.baseArrayLayer = 0;
  m_sub_range.layerCount = 1;

  if (GetType() == TextureType::kImageTexture) {
    m_2d_sampler = m_ctx->Get2DSampler();
  }
}

void Texture::CleanUp() {
  if (m_image) {
    vmaDestroyImage(m_allocator, m_image, m_allocation_info);

    m_image = VK_NULL_HANDLE;
  }

  if (m_image_view) {
    vkDestroyImageView(m_ctx->GetDevice(), m_image_view, nullptr);

    m_image_view = VK_NULL_HANDLE;
  }
}

void Texture::PrepareForDraw() {
  if (m_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    return;
  }

  auto cmd = m_ctx->ObtainCommandBuffer();

  TransforImageLayout(cmd, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  m_ctx->SubmitCommandBuffer(cmd);
}

void Texture::OnResize(uint32_t old_w, uint32_t old_h, uint32_t new_w, uint32_t new_h) {
  if (old_w != new_w || old_h != new_h) {
    CleanUp();

    InitImage(new_w, new_h);
  }
}

void Texture::OnUploadData(uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height, void* data) {
  size_t buffer_size = width * height * vk_format_comp(m_vk_format);

  if (buffer_size == 0) {
    HEX_CORE_WARN("Try upload zero buffer to texture");
    return;
  }

  std::unique_ptr<StageBuffer> stage_buffer = std::make_unique<StageBuffer>(m_allocator);

  stage_buffer->Init(buffer_size);

  stage_buffer->UploadData(data, buffer_size);

  VkCommandBuffer cmd = m_ctx->ObtainCommandBuffer();

  // step 2 set vk_image layout for translate
  if (m_image_layout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    TransforImageLayout(cmd, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  }

  // step 3 transfer image data from stage buffer to image buffer
  VkBufferImageCopy copy_region = {};
  copy_region.bufferOffset = 0;
  copy_region.bufferRowLength = 0;
  copy_region.bufferImageHeight = 0;

  copy_region.imageSubresource.aspectMask = m_sub_range.aspectMask;
  copy_region.imageSubresource.mipLevel = 0;
  copy_region.imageSubresource.baseArrayLayer = 0;
  copy_region.imageSubresource.layerCount = 1;
  copy_region.imageOffset = {(int32_t)offset_x, (int32_t)offset_y, 0};
  copy_region.imageExtent = {width, height, 1};

  CopyBufferToImage(cmd, stage_buffer->NativeBuffer(), copy_region);

  m_ctx->SubmitCommandBuffer(cmd);
}

void Texture::InitImage(uint32_t width, uint32_t height) {
  m_image_extent.width = width;
  m_image_extent.height = height;
  m_image_extent.depth = 1;

  m_image_layout = VK_IMAGE_LAYOUT_UNDEFINED;

  VkImageCreateInfo image_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};

  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.extent = m_image_extent;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage = m_vk_usage;
  image_info.format = m_vk_format;

  VmaAllocationCreateInfo vma_info{};
  vma_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  vma_info.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

  if (vmaCreateImage(m_allocator, &image_info, &vma_info, &m_image, &m_allocation_info, nullptr) != VK_SUCCESS) {
    HEX_CORE_ERROR("Failed allocate image texture for size: [{}, {}]", width, height);
    exit(-1);
  }

  VkImageViewCreateInfo view_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};

  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = m_vk_format;
  view_info.image = m_image;
  view_info.subresourceRange = m_sub_range;

  vkCreateImageView(m_ctx->GetDevice(), &view_info, nullptr, &m_image_view);
}

void Texture::TransforImageLayout(VkCommandBuffer cmd, VkImageLayout new_layout) {
  VkPipelineStageFlags src_stage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
  VkPipelineStageFlags dst_stage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

  VkImageMemoryBarrier image_memory_barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  image_memory_barrier.oldLayout = m_image_layout;
  image_memory_barrier.newLayout = new_layout;
  image_memory_barrier.image = m_image;
  image_memory_barrier.subresourceRange = m_sub_range;

  // source layouts(old)
  // source access mask controls that have to be finished on the old layout
  // before it will be transitioned to the new layout
  switch (m_image_layout) {
    case VK_IMAGE_LAYOUT_UNDEFINED:
    case VK_IMAGE_LAYOUT_GENERAL:
      // Image layout is undefined(or does not matter)
      // Only valid as init layout
      // No flags required, listed only for completeness
      image_memory_barrier.srcAccessMask = 0;
      break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      // Image is preinitialized
      // Only valid as initial layout for linear images, preserves memory
      // contents Make sure host writes have been finished
      image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image is a color attachment
      // Make sure any writes to the color buffer have been finished
      image_memory_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image is depth/stencil attachment
      // Make sure any writes to the depth/stencil buffer have been finished
      image_memory_barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image is a transfer source
      // Make sure any reads from the images have been finished
      image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image is a transfer destination
      // Make sure any writes to the image have been finished
      break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image is read by a shader
      // Make sure any shader reads from the image have been finished
      image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;
    default:
      HEX_CORE_WARN("Other source layouts aren't handled, and may cause error in vulkan");
      break;
  }

  // Target layouts(new)
  // Destination access mask controls the dependency for the new image layout
  switch (new_layout) {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image will be used as a transfer destination
      // Make sure any writes to the image have been finished
      image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image will be used as a transfer source
      // Make sure any reads from the image have been finished
      image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image will be used as a color attachment
      // Make sure any writes to the color buffer have been finished
      image_memory_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image layout will be used as a depth/stencil attachment
      // Make sure any writes to depth/stencil buffer have been finished
      image_memory_barrier.dstAccessMask =
          image_memory_barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image will be read in a shader (sampler, input attachment)
      // Make sure any writes to the image have been finished
      if (image_memory_barrier.srcAccessMask == 0) {
        image_memory_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
      }
      image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;
    case VK_IMAGE_LAYOUT_GENERAL:
      image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
      image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;
    default:
      HEX_CORE_WARN("Other target layouts aren't handled (yet)");
      break;
  }

  vkCmdPipelineBarrier(cmd, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);

  m_image_layout = new_layout;
}

void Texture::CopyBufferToImage(VkCommandBuffer cmd, VkBuffer vk_buffer, VkBufferImageCopy const& copy_info) {
  vkCmdCopyBufferToImage(cmd, vk_buffer, m_image, m_image_layout, 1, &copy_info);
}

}  // namespace hexgon::gpu::vk
