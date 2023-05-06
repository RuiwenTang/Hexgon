#include "src/render/vulkan/swapchain_vk.hpp"

namespace Hexgon {

SwapchainVK::SwapchainVK(const SwapchainDescriptor& desc, VkSwapchainKHR vk_sh)
    : Swapchain(desc), m_vk_swapchain(vk_sh) {}

SwapchainVK::~SwapchainVK() {}

bool SwapchainVK::Init() { return false; }

void SwapchainVK::Shutdown() {}

glm::ivec2 SwapchainVK::GetResolution() { return glm::ivec2(); }

Scope<RenderTarget> SwapchainVK::AcquireNextFrame() {
  return Scope<RenderTarget>();
}

RenderPass* SwapchainVK::GetRenderPass() { return nullptr; }

void SwapchainVK::Present() {}

}  // namespace Hexgon
