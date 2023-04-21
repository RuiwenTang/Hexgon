#pragma once

#include <glm/glm.hpp>
#include <hexgon/core/base.hpp>
#include <hexgon/render/render_target.hpp>

namespace Hexgon {

struct SwapchainDescriptor {
  // color bites
  uint32_t color = 32;
  uint32_t depth = 24;
  uint32_t stencil = 8;
  uint32_t samples = 1;
};

class Swapchain {
 public:
  Swapchain(const SwapchainDescriptor& desc) : m_desc(desc) {}
  virtual ~Swapchain() = default;

  virtual bool Init() = 0;

  virtual void Shutdown() = 0;

  virtual glm::ivec2 GetResolution() = 0;

  virtual Scope<RenderTarget> AcquireNextFrame() = 0;

  virtual void Present() = 0;

  const SwapchainDescriptor& GetDescriptor() const { return m_desc; }

 private:
  SwapchainDescriptor m_desc;
};

}  // namespace Hexgon
