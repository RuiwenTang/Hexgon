#include "src/render/vulkan/swapchain_vk.hpp"

namespace Hexgon {

SwapchainVK::SwapchainVK(const SwapchainDescriptor& desc, VkSwapchainKHR vk_sh)
    : Swapchain(desc), m_vk_swapchain(vk_sh) {}

SwapchainVK::~SwapchainVK() {}

}  // namespace Hexgon
