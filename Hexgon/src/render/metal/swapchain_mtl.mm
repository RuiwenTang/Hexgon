#include "src/render/metal/swapchain_mtl.h"
#include "src/render/metal/renderpass_mtl.h"
#include "src/render/metal/texture_mtl.h"
#include "src/render/metal/types_mtl.h"

namespace Hexgon {

SwapchainMTL::SwapchainMTL(const SwapchainDescriptor& desc,
                           RenderPassMTL* renderPass, MTKView* view)
    : Swapchain(desc), m_render_pass(renderPass), m_view(view) {}

SwapchainMTL::~SwapchainMTL() {}

bool SwapchainMTL::Init() { return true; }

void SwapchainMTL::Shutdown() {}

glm::ivec2 SwapchainMTL::GetResolution() {
  return glm::ivec2{m_view.frame.size.width, m_view.frame.size.height};
}

Scope<RenderTarget> SwapchainMTL::AcquireNextFrame() {
  id<CAMetalDrawable> drawable = [m_view currentDrawable];

  id<MTLTexture> color_texture = drawable.texture;

  id<MTLTexture> depth_stencil_texture = m_view.depthStencilTexture;

  RenderTargetDescriptor desc{};

  TextureDescriptor color_desc{};

  color_desc.format =
      MTL::Convertor<Format, MTLPixelFormat>::ToGPU(color_texture.pixelFormat);
  color_desc.size =
      glm::ivec2{m_view.frame.size.width, m_view.frame.size.height};

  color_desc.bindFlags = BindFlags::kColorAttachment;

  desc.attachments.emplace_back(
      AttachmentReference{0, CreateRef<TextureMTL>(color_desc, color_texture),
                          BindFlags::kColorAttachment});

  if (depth_stencil_texture) {
    TextureDescriptor depth_desc{};
    depth_desc.format = MTL::Convertor<Format, MTLPixelFormat>::ToGPU(
        depth_stencil_texture.pixelFormat);
    depth_desc.size = color_desc.size;
    depth_desc.bindFlags = BindFlags::kDepthStencilAttachment;

    desc.attachments.emplace_back(AttachmentReference{
        0, CreateRef<TextureMTL>(depth_desc, depth_stencil_texture),
        BindFlags::kDepthStencilAttachment});
  }

  m_curr_drawable = drawable;
  return CreateScope<RenderTarget>(desc);
}

void SwapchainMTL::Present() {
  [m_curr_drawable present];
  m_curr_drawable = nil;

  [m_view draw];
}

}
