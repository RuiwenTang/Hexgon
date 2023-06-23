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

#include <Hexgon/Render/RenderSystem.hpp>
#include <vector>

#include "Render/Vulkan/VulkanUtil.hpp"

namespace hexgon {

class RenderSystemVk : public RenderSystem {
 public:
  RenderSystemVk() = default;
  ~RenderSystemVk() override = default;

  static std::unique_ptr<RenderSystem> Init(Window* window);

  virtual std::unique_ptr<SwapChain> CreateSwapChain() override;

  virtual void ShutDown() override;

  // platform functions
  bool InitVulkan(VkInstance instance, VkSurfaceKHR surface, const PhysicalDeviceInfo& device_info);

 private:
  VkInstance m_vk_instance = {};
  VkSurfaceKHR m_vk_surface = {};
  VkPhysicalDevice m_phy_device = {};
  uint32_t m_graphic_queue_index = {};
  uint32_t m_present_queue_index = {};
  VkDevice m_device = {};
  VkQueue m_graphic_queue = {};
  VkQueue m_present_queue = {};
};

}  // namespace hexgon
