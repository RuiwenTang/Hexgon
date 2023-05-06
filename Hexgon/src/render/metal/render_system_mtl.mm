#include "src/render/metal/render_system_mtl.hpp"

#include <hexgon/core/window.hpp>

#include <AppKit/AppKit.h>
#include <GLFW/glfw3.h>
#include <QuartzCore/QuartzCore.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <MetalKit/MetalKit.h>

#include "src/render/metal/command_buffer_mtl.h"
#include "src/render/metal/renderpass_mtl.h"
#include "src/render/metal/shader_mtl.h"
#include "src/render/metal/swapchain_mtl.h"
#include "src/render/metal/texture_mtl.h"

namespace Hexgon {

class RenderSystemMTL : public RenderSystem {
 public:
  RenderSystemMTL() = default;

  ~RenderSystemMTL() override = default;

  bool Init() override;

  void Shutdown() override;

  void BeginFrame() override;

  void EndFrame() override;

  Scope<Swapchain> CreateSwapchain(Window *window,
                                   const SwapchainDescriptor &desc) override {
    NSWindow *native_window =
        glfwGetCocoaWindow((GLFWwindow *)window->GetNativeWindow());

    MTKView *view = [[MTKView alloc] initWithFrame:native_window.frame
                                            device:m_device];

    auto render_pass_mtl =
        RenderPassMTL::CreateScreenRenderPass(m_device, desc);

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

    return CreateScope<SwapchainMTL>(desc, std::move(render_pass_mtl), view);
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

  Ref<Shader> CreateShader(const ShaderDescriptor &desc) override {
    auto shader = CreateRef<ShaderMTL>(desc, m_device);

    if (!shader->Compile()) {
      return nullptr;
    }

    return shader;
  }

  Ref<CommandBuffer> CreateCommandBuffer(
      const CommandBufferDescriptor &desc) override {
    return CreateRef<CommandBufferMTL>(desc, m_graphic_queue);
  }

  Ref<PipelineState> CreateGraphicPipelineState(
      const GraphicPipelineDescriptor &desc) override {
    return nullptr;
  }

  void Submit(const Ref<CommandBuffer> &cmd) override {
    auto mtl_cmd = dynamic_cast<CommandBufferMTL *>(cmd.get());

    mtl_cmd->CommitMTL();
  }

 private:
  id<MTLDevice> m_device = nil;
  id<MTLCommandQueue> m_graphic_queue = nil;
  NSAutoreleasePool *m_pool = nil;
};

bool RenderSystemMTL::Init() {
  // pick device
  m_device = MTLCreateSystemDefaultDevice();
  if (m_device == nil) {
    return false;
  }

  m_graphic_queue = [m_device newCommandQueue];

  return true;
}

void RenderSystemMTL::Shutdown() {
  if (m_pool) {
    [m_pool drain];
  }
}

void RenderSystemMTL::BeginFrame() {
  m_pool = [[NSAutoreleasePool alloc] init];
}

void RenderSystemMTL::EndFrame() {
  [m_pool drain];
  m_pool = nil;
}

Scope<RenderSystem> LoadMetalRender() {
  auto render_system = CreateScope<RenderSystemMTL>();

  return render_system;
}

}  // namespace Hexgon
