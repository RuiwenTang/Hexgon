#pragma once

#include <MetalKit/MetalKit.h>

#include <hexgon/render/command_buffer.hpp>
#include <vector>

namespace Hexgon {

class CommandBufferMTL : public CommandBuffer {
 public:
  CommandBufferMTL(const CommandBufferDescriptor& desc,
                   id<MTLCommandQueue> queue);
  ~CommandBufferMTL() override = default;

  void Begin() override;

  void End() override;

  bool BeginRenderPass(RenderPass* renderPass, RenderTarget* renderTarget,
                       const std::vector<AttachmentClear>& clearValue) override;

  void EndRenderPass() override;

  void CommitMTL();

 private:
  id<MTLCommandQueue> m_queue;
  id<MTLCommandBuffer> m_current_cmd = nil;
  id<MTLCommandEncoder> m_curr_encoder = nil;
  std::vector<id<MTLDrawable>> m_drawables = {};
};

}  // namespace Hexgon