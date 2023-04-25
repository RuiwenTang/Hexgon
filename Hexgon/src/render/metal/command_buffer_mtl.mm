#include "src/render/metal/command_buffer_mtl.h"
#include "src/render/metal/renderpass_mtl.h"
#include "src/render/metal/texture_mtl.h"

namespace Hexgon {

CommandBufferMTL::CommandBufferMTL(const CommandBufferDescriptor& desc,
                                   id<MTLCommandQueue> queue)
    : CommandBuffer(desc), m_queue(queue) {}

void CommandBufferMTL::Begin() {
  if (m_current_cmd) {
    End();

    [m_current_cmd commit];
  }

  m_current_cmd = [m_queue commandBuffer];
}

void CommandBufferMTL::End() {
  for (auto d : m_drawables) {
    [m_current_cmd presentDrawable:d];
  }

  [m_current_cmd enqueue];
}

static MTLClearColor to_mtl_clear(const ClearValue& value) {
  return MTLClearColor{
      (double)value.color[0],
      (double)value.color[1],
      (double)value.color[2],
      (double)value.color[3],
  };
}

bool CommandBufferMTL::BeginRenderPass(
    RenderPass* renderPass, RenderTarget* renderTarget,
    const std::vector<AttachmentClear>& clearValue) {
  if (!m_current_cmd) {
    return false;
  }

  auto renderPassMTL = dynamic_cast<RenderPassMTL*>(renderPass);
  if (renderPassMTL == nullptr) {
    return false;
  }

  MTLRenderPassDescriptor* mtl_render_pass =
      [MTLRenderPassDescriptor renderPassDescriptor];

  const auto& color_attachments = renderPassMTL->GetColorAttachment();
  const auto& color_ref = renderTarget->GetAttachments();

  size_t clear_index = 0;

  for (size_t i = 0; i < color_attachments.size(); i++) {
    MTLRenderPassColorAttachmentDescriptor* desc =
        [MTLRenderPassColorAttachmentDescriptor new];

    auto mtl_tex = dynamic_cast<TextureMTL*>(color_ref[i].texture.get());

    desc.loadAction = color_attachments[i].load;
    desc.storeAction = color_attachments[i].store;
    desc.texture = mtl_tex->GetNativeTexture();

    if (clear_index < clearValue.size()) {
      desc.clearColor = to_mtl_clear(clearValue[clear_index].value);
    } else {
      desc.clearColor = MTLClearColor{};
    }

    [mtl_render_pass.colorAttachments setObject:desc atIndexedSubscript:i];

    clear_index++;
  }

  const auto& depth_attachment = renderPassMTL->GetGetDepthAttachment();
  const auto& depth_ref = renderTarget->GetDepthStencilAttachment();

  if (depth_attachment) {
    if (!depth_ref.texture) {
      return false;
    }

    auto mtl_tex = dynamic_cast<TextureMTL*>(depth_ref.texture.get());

    MTLRenderPassDepthAttachmentDescriptor* desc =
        [MTLRenderPassDepthAttachmentDescriptor new];
    desc.loadAction = depth_attachment->load;
    desc.storeAction = depth_attachment->store;
    desc.texture = mtl_tex->GetNativeTexture();

    if (clear_index < clearValue.size()) {
      desc.clearDepth = clearValue[clear_index].value.depth;
    }

    mtl_render_pass.depthAttachment = desc;
  }

  const auto& stencil_attachment = renderPassMTL->GetStencilAttachment();
  if (stencil_attachment) {
    if (!depth_ref.texture) {
      return false;
    }

    auto mtl_tex = dynamic_cast<TextureMTL*>(depth_ref.texture.get());

    MTLRenderPassStencilAttachmentDescriptor* desc =
        [MTLRenderPassStencilAttachmentDescriptor new];

    desc.loadAction = stencil_attachment->load;
    desc.storeAction = stencil_attachment->store;
    desc.texture = mtl_tex->GetNativeTexture();

    if (clear_index < clearValue.size()) {
      desc.clearStencil = clearValue[clear_index].value.stencil;
    }

    mtl_render_pass.stencilAttachment = desc;
  }

  m_curr_encoder =
      [m_current_cmd renderCommandEncoderWithDescriptor:mtl_render_pass];

  return true;
}

void CommandBufferMTL::EndRenderPass() {
  if (!m_curr_encoder) {
    return;
  }

  [m_curr_encoder endEncoding];
  m_curr_encoder = nil;
}

void CommandBufferMTL::CommitMTL() {
  if (m_current_cmd == nil) {
    return;
  }

  [m_current_cmd commit];

  m_current_cmd = nil;
}

}
