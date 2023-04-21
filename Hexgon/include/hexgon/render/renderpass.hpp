#pragma once

#include <glm/glm.hpp>
#include <hexgon/render/format.hpp>
#include <optional>
#include <vector>

namespace Hexgon {

enum class AttachmentLoadOp {
  kNotCare,
  kClear,
  kLoad,
};

enum class AttachmentStoreOp {
  kNotCare,
  kStore,
};

struct AttachmentDescriptor {
  Format format;
  AttachmentLoadOp load;
  AttachmentStoreOp store;

  AttachmentDescriptor(Format format, AttachmentLoadOp load,
                       AttachmentStoreOp store)
      : format(format), load(load), store(store) {}
};

struct RenderPassDescriptor {
  std::vector<AttachmentDescriptor> color_attachments = {};

  std::optional<AttachmentDescriptor> depth_attachment = {};

  std::optional<AttachmentDescriptor> stencil_attachment = {};
};

class RenderPass {
 public:
  RenderPass(const RenderPassDescriptor& desc) : m_desc(desc) {}

  virtual ~RenderPass() = default;

  const RenderPassDescriptor& GetDescriptor() const { return m_desc; }

 private:
  RenderPassDescriptor m_desc;
};

}  // namespace Hexgon