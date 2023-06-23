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
    : m_device(device), m_vk_swap_chain(swap_chain), m_caps(caps), m_format(format), frame_data() {
  InitInternal();
}

SwapChainVk::~SwapChainVk() { DestroyInternal(); }

uint32_t SwapChainVk::GetWidth() const { return m_caps.currentExtent.width; }

uint32_t SwapChainVk::GetHeight() const { return m_caps.currentExtent.height; }

uint32_t SwapChainVk::GetMaxBufferCount() const { return m_caps.maxImageCount; }

void SwapChainVk::InitInternal() {
  // init per frame datas
  frame_data.resize(GetMaxBufferCount());

  for (auto& data : frame_data) {
    data.Init(m_device, 0);
  }
}

void SwapChainVk::DestroyInternal() { frame_data.clear(); }

}  // namespace hexgon
