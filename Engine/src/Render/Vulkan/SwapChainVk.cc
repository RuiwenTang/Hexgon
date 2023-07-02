/*
 *   Copyright (c) 2023 RuiwenTang
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

#include "Render/Vulkan/SwapChainVk.hpp"

namespace hexgon {

PerFrameData::~PerFrameData() {
  // reset pool first
  if (cmd_pool) {
    vkResetCommandPool(device, cmd_pool, 0);
  }
  // release cmd first
  if (cmd) {
    vkFreeCommandBuffers(device, cmd_pool, 1, &cmd);
    cmd = nullptr;
  }
  // destroy pool
  if (cmd_pool) {
    vkDestroyCommandPool(device, cmd_pool, nullptr);
    cmd_pool = nullptr;
  }
  // destroy fence
  if (submit_fence) {
    vkDestroyFence(device, submit_fence, nullptr);
    submit_fence = nullptr;
  }

  // semaphore
  if (acquire_semaphore) {
    vkDestroySemaphore(device, acquire_semaphore, nullptr);
    acquire_semaphore = nullptr;
  }
  if (release_semaphore) {
    vkDestroySemaphore(device, release_semaphore, nullptr);
    release_semaphore = nullptr;
  }
}

void PerFrameData::Init(VkDevice device, uint32_t queue_index) {
  this->device = device;
  // fence
  {
    VkFenceCreateInfo info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(this->device, &info, nullptr, &this->submit_fence);
  }
  // command pool
  {
    VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    info.queueFamilyIndex = queue_index;

    vkCreateCommandPool(this->device, &info, nullptr, &this->cmd_pool);
  }

  // command buffer
  {
    VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    info.commandPool = cmd_pool;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = 1;

    vkAllocateCommandBuffers(this->device, &info, &this->cmd);
  }
}

SwapChainVk::SwapChainVk(VkDevice device, VkSwapchainKHR swap_chain, VkSurfaceCapabilitiesKHR caps, VkFormat format)
    : m_device(device),
      m_vk_swap_chain(swap_chain),
      m_caps(caps),
      m_format(format),
      m_swap_chain_images(),
      m_swap_chain_image_views(),
      m_frame_data() {
  InitInternal();
}

SwapChainVk::~SwapChainVk() { DestroyInternal(); }

uint32_t SwapChainVk::GetWidth() const { return m_caps.currentExtent.width; }

uint32_t SwapChainVk::GetHeight() const { return m_caps.currentExtent.height; }

uint32_t SwapChainVk::GetMaxBufferCount() const { return m_caps.maxImageCount; }

void SwapChainVk::InitInternal() {
  // init per frame datas
  m_frame_data.resize(GetMaxBufferCount());

  for (auto& data : m_frame_data) {
    data.Init(m_device, 0);
  }

  // all image buffers in swapchain
  m_swap_chain_images.resize(GetMaxBufferCount());

  uint32_t image_count = 0;
  vkGetSwapchainImagesKHR(m_device, m_vk_swap_chain, &image_count, m_swap_chain_images.data());

  m_swap_chain_image_views.resize(image_count);

  for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
    // Create image view for swapchain images
    VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    info.image = m_swap_chain_images[i];
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = m_format;
    info.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
    info.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    vkCreateImageView(m_device, &info, nullptr, &m_swap_chain_image_views[i]);
  }
}

void SwapChainVk::DestroyInternal() {
  m_frame_data.clear();

  // relse image views
  for (auto& image_view : m_swap_chain_image_views) {
    vkDestroyImageView(m_device, image_view, nullptr);
  }

  m_swap_chain_image_views.clear();

  m_swap_chain_images.clear();

  vkDestroySwapchainKHR(m_device, m_vk_swap_chain, nullptr);
}

}  // namespace hexgon
