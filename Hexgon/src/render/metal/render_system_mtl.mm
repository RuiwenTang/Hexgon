#include "src/render/metal/render_system_mtl.hpp"

#include <hexgon/core/window.hpp>

#include <AppKit/AppKit.h>
#include <GLFW/glfw3.h>
#include <QuartzCore/QuartzCore.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <MetalKit/MetalKit.h>

#include "src/render/metal/renderpass_mtl.h"
#include "src/render/metal/swapchain_mtl.h"
#include "src/render/metal/texture_mtl.h"

namespace Hexgon {

class RenderSystemMTL : public RenderSystem {
 public:
  RenderSystemMTL() = default;

  ~RenderSystemMTL() override = default;

  bool Init() override;

  void Shutdown() override;

  Scope<Swapchain> CreateSwapchain(Window *window,
                                   const SwapchainDescriptor &desc,
                                   RenderPass *renderPass) override {
    NSWindow *native_window =
        glfwGetCocoaWindow((GLFWwindow *)window->GetNativeWindow());

    MTKView *view = [[MTKView alloc] initWithFrame:native_window.frame
                                            device:m_device];

    auto render_pass_mtl = dynamic_cast<RenderPassMTL *>(renderPass);

    auto const &color_attachments = render_pass_mtl->GetColorAttachment();

    if (color_attachments.empty()) {
      return Scope<Swapchain>();
    }

    view.colorPixelFormat = color_attachments[0].format;

    auto const &depth_attachment = render_pass_mtl->GetGetDepthAttachment();
    auto const &stencil_attachment = render_pass_mtl->GetStencilAttachment();

    if (depth_attachment) {
      view.depthStencilPixelFormat = depth_attachment->format;
    }

    if (stencil_attachment) {
      view.depthStencilPixelFormat = stencil_attachment->format;
    }

    [native_window setContentView:view];
    [native_window.contentViewController setView:view];

    return CreateScope<SwapchainMTL>(desc, render_pass_mtl, view);
  }

  Scope<RenderPass> CreateRenderPass(
      const RenderPassDescriptor &desc) override {
    RenderPassDescriptor copy = desc;

    if (copy.depth_attachment || copy.stencil_attachment) {
      Format ds_format = copy.depth_attachment.has_value()
                             ? copy.depth_attachment->format
                             : copy.stencil_attachment->format;

      if (ds_format == Format::D24UNormS8UInt &&
          !m_device.depth24Stencil8PixelFormatSupported) {
        ds_format = Format::D32FloatS8X24UInt;
      }

      if (copy.depth_attachment) {
        copy.depth_attachment->format = ds_format;
      }

      if (copy.stencil_attachment) {
        copy.stencil_attachment->format = ds_format;
      }
    }

    return CreateScope<RenderPassMTL>(copy);
  }

  Ref<Texture> CreateTexture(const TextureDescriptor &desc) override {
    return nullptr;
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
