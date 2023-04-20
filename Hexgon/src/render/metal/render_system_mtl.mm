#include "src/render/metal/render_system_mtl.hpp"

#include <hexgon/core/window.hpp>
#include "hexgon/core/base.hpp"

#include <AppKit/AppKit.h>
#include <GLFW/glfw3.h>
#include <QuartzCore/QuartzCore.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <MetalKit/MetalKit.h>

#include "src/render/metal/swapchain_mtl.h"

namespace Hexgon {

class RenderSystemMTL : public RenderSystem {
 public:
  RenderSystemMTL() = default;

  ~RenderSystemMTL() override = default;

  bool Init() override;

  void Shutdown() override;

  Scope<Swapchain> CreateSwapchain(Window *window,
                                   const SwapchainDescriptor &desc) override {
    NSWindow *native_window =
        glfwGetCocoaWindow((GLFWwindow *)window->GetNativeWindow());

    MTKView *view = [[MTKView alloc] initWithFrame:native_window.frame
                                            device:m_device];

    [native_window setContentView:view];

    return CreateScope<SwapchainMTL>(desc, view);
  }

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
