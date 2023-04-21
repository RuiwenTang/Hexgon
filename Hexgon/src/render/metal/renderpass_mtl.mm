#include "src/render/metal/renderpass_mtl.h"

#include "hexgon/render/format.hpp"
#include "hexgon/render/renderpass.hpp"
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

}
