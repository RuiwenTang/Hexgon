#pragma once

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include <hexgon/render/renderpass.hpp>

namespace Hexgon {

struct MTLAttachmentDesc {
  MTLPixelFormat format = MTLPixelFormatInvalid;
  MTLLoadAction load = MTLLoadActionDontCare;
  MTLStoreAction store = MTLStoreActionDontCare;
};

class RenderPassMTL : public RenderPass {
 public:
  RenderPassMTL(const RenderPassDescriptor& desc) : RenderPass(desc) {
    InitAttachments();
  }

  ~RenderPassMTL() override = default;

  const std::vector<MTLAttachmentDesc>& GetColorAttachment() const {
    return mtl_color_attachments;
  }

  const std::optional<MTLAttachmentDesc>& GetGetDepthAttachment() const {
    return mtl_depth_attachment;
  }

  const std::optional<MTLAttachmentDesc>& GetStencilAttachment() const {
    return mtl_stencil_attachment;
  }

 private:
  void InitAttachments();

 private:
  std::vector<MTLAttachmentDesc> mtl_color_attachments = {};
  std::optional<MTLAttachmentDesc> mtl_depth_attachment = {};
  std::optional<MTLAttachmentDesc> mtl_stencil_attachment = {};
  MTLRenderPassDescriptor* m_mtl_desc = nil;
};

}  // namespace Hexgon
