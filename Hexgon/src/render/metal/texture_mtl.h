#pragma once

#include <MetalKit/MetalKit.h>

#include <hexgon/core/base.hpp>
#include <hexgon/render/texture.hpp>

namespace Hexgon {

class TextureMTL : public Texture {
 public:
  TextureMTL(const TextureDescriptor& desc, id<MTLTexture> texture);

  ~TextureMTL() override;

  static Ref<TextureMTL> CreateTexture(const TextureDescriptor& desc,
                                       id<MTLDevice> gpu);

  id<MTLTexture> GetNativeTexture() const { return m_native_texture; }

 private:
  id<MTLTexture> m_native_texture;
};

}  // namespace Hexgon
