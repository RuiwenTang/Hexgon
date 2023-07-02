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

#pragma once

#include <vulkan/vulkan.h>

#include <Hexgon/Render/SwapChain.hpp>
#include <vector>

namespace hexgon {

struct PerFrameData {
  VkDevice device = {};
  VkFence submit_fence = {};
  VkCommandPool cmd_pool = {};
  VkCommandBuffer cmd = {};
  VkSemaphore acquire_semaphore = {};
  VkSemaphore release_semaphore = {};

  PerFrameData() = default;

  ~PerFrameData();

  void Init(VkDevice device, uint32_t queue_index);
};

class SwapChainVk : public SwapChain {
 public:
  SwapChainVk(VkDevice device, VkSwapchainKHR swap_chain, VkSurfaceCapabilitiesKHR caps, VkFormat format);

  ~SwapChainVk() override;

  virtual uint32_t GetWidth() const override;

  virtual uint32_t GetHeight() const override;

  virtual uint32_t GetMaxBufferCount() const override;

 private:
  void InitInternal();

  void DestroyInternal();

 private:
  VkDevice m_device = {};
  VkSwapchainKHR m_vk_swap_chain = {};
  VkSurfaceCapabilitiesKHR m_caps = {};
  VkFormat m_format = {};

  std::vector<VkImage> m_swap_chain_images = {};
  std::vector<VkImageView> m_swap_chain_image_views = {};
  std::vector<PerFrameData> m_frame_data = {};
};

}  // namespace hexgon
