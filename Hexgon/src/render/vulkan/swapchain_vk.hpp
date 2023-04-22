#pragma once

#include <vulkan/vulkan.h>

#include <hexgon/render/swapchain.hpp>

namespace Hexgon {

class SwapchainVK : public Swapchain {
 public:
  SwapchainVK(const SwapchainDescriptor& desc, VkSwapchainKHR vk_sh);
  ~SwapchainVK() override;

 private:
  VkSwapchainKHR m_vk_swapchain = VK_NULL_HANDLE;
};

}  // namespace Hexgon
