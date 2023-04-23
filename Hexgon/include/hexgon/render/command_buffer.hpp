#pragma once

#include <glm/glm.hpp>
#include <hexgon/render/render_target.hpp>
#include <hexgon/render/renderpass.hpp>
#include <vector>

namespace Hexgon {

struct ClearValue {
  float color[4] = {0.f};
  float depth = 0.f;
  uint32_t stencil = 0;

  ClearValue() = default;
  ClearValue(float r, float g, float b, float a) : color{r, g, b, a} {}
  ClearValue(const glm::vec4& c) : color{c.r, c.g, c.b, c.a} {}
  ClearValue(float d, uint32_t s) : depth(d), stencil(s) {}
};

enum class ClearFlags {
  kColor,
  kDepthStencil,
};

struct AttachmentClear {
  ClearFlags flag = ClearFlags::kColor;
  ClearValue value = {};
};

enum class CommandBufferFlag {
  kMultiSubmit,
  kImmediateSubmit,
};

struct CommandBufferDescriptor {
  CommandBufferFlag flag = CommandBufferFlag::kImmediateSubmit;
  bool secondary = false;
};

class CommandBuffer {
 public:
  CommandBuffer(const CommandBufferDescriptor& desc) : m_desc(desc) {}

  virtual ~CommandBuffer() = default;

  virtual void Begin() = 0;

  virtual void End() = 0;

  virtual bool BeginRenderPass(
      RenderPass* renderPass, RenderTarget* renderTarget,
      const std::vector<AttachmentClear>& clearValue) = 0;

  virtual void EndRenderPass() = 0;

 private:
  CommandBufferDescriptor m_desc;
};

}  // namespace Hexgon
