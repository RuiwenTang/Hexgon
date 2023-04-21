#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/render/renderpass.hpp>
#include <hexgon/render/swapchain.hpp>

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
};

}  // namespace Hexgon
