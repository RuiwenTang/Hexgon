#pragma once

#include <glm/glm.hpp>
#include <hexgon/core/base.hpp>
#include <hexgon/render/texture.hpp>
#include <hexgon/render/types.hpp>
#include <vector>

namespace Hexgon {

class RenderPass;

struct AttachmentReference {
  // index in attachment descriptors
  uint32_t attachment = 0;
  // actual GPU texture
  Ref<Texture> texture = {};
  // usage
  BindFlags flags = BindFlags::kUndefined;
};

struct RenderTargetDescriptor {
  RenderPass* renderPass = nullptr;

  std::vector<AttachmentReference> attachments = {};

  glm::ivec2 resolution = {};
};

class RenderTarget {
 public:
  RenderTarget(const RenderTargetDescriptor& desc) : m_desc(desc) {}
  virtual ~RenderTarget() = default;

  const std::vector<AttachmentReference>& GetAttachments() const {
    return m_desc.attachments;
  }

  RenderPass* GetRenderPass() const { return m_desc.renderPass; }

  glm::ivec2 GetResolution() const { return m_desc.resolution; }

 private:
  RenderTargetDescriptor m_desc;
};

}  // namespace Hexgon