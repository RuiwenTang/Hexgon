#include "src/render/metal/render_system_mtl.hpp"

#include <hexgon/core/window.hpp>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <MetalKit/MetalKit.h>

namespace Hexgon {

class RenderSystemMTL : public RenderSystem {
 public:
  RenderSystemMTL() = default;

  ~RenderSystemMTL() override = default;

  bool Init() override;

  void Shutdown() override;

 private:
  id<MTLDevice> m_device = nil;
};

bool RenderSystemMTL::Init() {
  // pick device
  m_device = MTLCreateSystemDefaultDevice();
  if (m_device == nil) {
    return false;
  }

  return true;
}

void RenderSystemMTL::Shutdown() {}

Scope<RenderSystem> LoadMetalRender() {
  auto render_system = CreateScope<RenderSystemMTL>();

  return render_system;
}

}  // namespace Hexgon
