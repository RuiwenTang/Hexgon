#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <Hexgon/render/render_system.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Hexgon {

Scope<RenderSystem> LoadVulkanRender();

class RenderSystemVK : public RenderSystem {
 public:
  RenderSystemVK() = default;
  ~RenderSystemVK() override = default;

  bool Init() override;

  void Shutdown() override;

  Scope<Swapchain> CreateSwapchain(Window* window,
                                   const SwapchainDescriptor& desc,
                                   RenderPass* renderPass) override;

  Scope<RenderPass> CreateRenderPass(const RenderPassDescriptor& desc) override;

  Ref<Texture> CreateTexture(const TextureDescriptor& desc) override;

 private:
  bool InitVkInstance();
  bool PickDevice();

 private:
  VkInstance m_vk_ins = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_vk_debug_msg = VK_NULL_HANDLE;
  VkPhysicalDevice m_phy_dev = VK_NULL_HANDLE;
};

}  // namespace Hexgon