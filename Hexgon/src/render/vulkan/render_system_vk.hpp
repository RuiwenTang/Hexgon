#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <Hexgon/render/render_system.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vector>

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
  bool QueryDevices();

 private:
  VkInstance m_vk_ins = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_vk_debug_msg = VK_NULL_HANDLE;
  std::vector<VkPhysicalDevice> m_vk_gpus = {};
};

}  // namespace Hexgon