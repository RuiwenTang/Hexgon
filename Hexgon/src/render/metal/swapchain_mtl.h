
#pragma once

#include <MetalKit/MetalKit.h>

#include <hexgon/render/swapchain.hpp>

namespace Hexgon {

class RenderPassMTL;

class SwapchainMTL : public Swapchain {
 public:
  SwapchainMTL(const SwapchainDescriptor& desc, Scope<RenderPassMTL> renderPass,
               MTKView* view);
  ~SwapchainMTL() override;

  bool Init() override;

  void Shutdown() override;

  glm::ivec2 GetResolution() override;

  Scope<RenderTarget> AcquireNextFrame() override;

  RenderPass* GetRenderPass() override;

  void Present() override;

 private:
  Scope<RenderPassMTL> m_render_pass;
  MTKView* m_view;
  id<CAMetalDrawable> m_curr_drawable = nil;
};

}  // namespace Hexgon
