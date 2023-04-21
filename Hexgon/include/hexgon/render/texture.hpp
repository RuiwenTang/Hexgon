#pragma once

#include <glm/glm.hpp>
#include <hexgon/render/format.hpp>
#include <hexgon/render/types.hpp>

namespace Hexgon {

enum class TextureType {
  k1D,
  k2D,
  k3D,
  kCubic,
  k1DArray,
  k2DArray,
  kCubicArray,
  k2DMS,
  k3DMS,
};

struct TextureDescriptor {
  TextureType type = TextureType::k2D;

  Format format = Format::RGBA8UNorm;

  int bindFlags = BindFlags::kSampled | BindFlags::kColorAttachment;

  glm::ivec2 size = glm::ivec2{0, 0};

  int mipLevels = 0;
};

class Texture {
 public:
  Texture(const TextureDescriptor& desc) : m_desc(desc) {}
  virtual ~Texture() = default;

  const TextureDescriptor& GetDescriptor() const { return m_desc; }

 private:
  TextureDescriptor m_desc;
};

}  // namespace Hexgon
