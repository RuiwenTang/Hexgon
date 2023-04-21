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

 private:
  void InitAttachments();

 private:
  std::vector<MTLAttachmentDesc> mtl_color_attachments = {};
  std::optional<MTLAttachmentDesc> mtl_depth_attachment = {};
  std::optional<MTLAttachmentDesc> mtl_stencil_attachment = {};
};

}  // namespace Hexgon
