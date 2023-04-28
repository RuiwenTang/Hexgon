#include "src/render/metal/renderpass_mtl.h"

#include "src/render/metal/types_mtl.h"

namespace Hexgon {

static MTLAttachmentDesc convert_to_mtl(const AttachmentDescriptor& desc) {
  return MTLAttachmentDesc{
      MTL::Convertor<Format, MTLPixelFormat>::ToNative(desc.format),
      MTL::Convertor<AttachmentLoadOp, MTLLoadAction>::ToNative(desc.load),
      MTL::Convertor<AttachmentStoreOp, MTLStoreAction>::ToNative(desc.store),
  };
}

void RenderPassMTL::InitAttachments() {
  const auto& desc = GetDescriptor();

  // colors
  mtl_color_attachments.resize(desc.color_attachments.size());
  for (size_t i = 0; i < desc.color_attachments.size(); i++) {
    mtl_color_attachments[i] = convert_to_mtl(desc.color_attachments[i]);
  }

  // depth
  if (desc.depth_attachment) {
    mtl_depth_attachment = convert_to_mtl(desc.depth_attachment.value());
  }

  // stencil
  if (desc.stencil_attachment) {
    mtl_stencil_attachment = convert_to_mtl(desc.stencil_attachment.value());
  }
}

Scope<RenderPassMTL> RenderPassMTL::CreateScreenRenderPass(
    id<MTLDevice> gpu, const SwapchainDescriptor& desc) {
  RenderPassDescriptor rp_desc{};

  // TODO support MSAA resolve
  rp_desc.color_attachments.emplace_back(AttachmentDescriptor{
      Format::BGRA8UNorm,
      AttachmentLoadOp::kClear,
      AttachmentStoreOp::kStore,
  });

  Format depth_format = Format::D24UNormS8UInt;

  if (![gpu isDepth24Stencil8PixelFormatSupported]) {
    depth_format = Format::D32FloatS8X24UInt;
  }

  if (desc.depth != 0) {
    rp_desc.depth_attachment = AttachmentDescriptor{
        depth_format,
        AttachmentLoadOp::kClear,
        AttachmentStoreOp::kNotCare,
    };
  }

  if (desc.stencil != 0) {
    rp_desc.stencil_attachment = AttachmentDescriptor{
        depth_format,
        AttachmentLoadOp::kClear,
        AttachmentStoreOp::kNotCare,
    };
  }

  return CreateScope<RenderPassMTL>(rp_desc);
}

}
