
#pragma once

#include <MetalKit/MetalKit.h>

#include <hexgon/render/swapchain.hpp>

namespace Hexgon {

class SwapchainMTL : public Swapchain {
 public:
  SwapchainMTL(const SwapchainDescriptor& desc, MTKView* view);
  ~SwapchainMTL() override;

  bool Init() override;

  void Shutdown() override;

  glm::ivec2 GetResolution() override;

  void Present() override;

 private:
  MTKView* m_view;
};

}  // namespace Hexgon
