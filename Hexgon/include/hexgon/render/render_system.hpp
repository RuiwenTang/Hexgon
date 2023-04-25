#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/render/command_buffer.hpp>
#include <hexgon/render/renderpass.hpp>
#include <hexgon/render/swapchain.hpp>
#include <hexgon/render/texture.hpp>

namespace Hexgon {

class Window;

enum class RenderAPI {
  kNone,
  kOpenGL,
  kVulkan,
  kMetal,
};

class RenderSystem {
 public:
  virtual ~RenderSystem() = default;

  static Scope<RenderSystem> LoadRenderSystem(RenderAPI API);

  virtual bool Init() = 0;

  virtual void Shutdown() = 0;

  virtual Scope<Swapchain> CreateSwapchain(Window* window,
                                           const SwapchainDescriptor& desc) = 0;

  virtual Scope<RenderPass> CreateRenderPass(
      const RenderPassDescriptor& desc) = 0;

  virtual Ref<Texture> CreateTexture(const TextureDescriptor& desc) = 0;

  virtual Ref<CommandBuffer> CreateCommandBuffer(
      const CommandBufferDescriptor& desc) = 0;

  virtual void Submit(const Ref<CommandBuffer>& cmd) = 0;
};

}  // namespace Hexgon
