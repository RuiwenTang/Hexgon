
#include "src/render/metal/texture_mtl.h"
#include <Metal/Metal.h>
#include "hexgon/render/texture.hpp"

namespace Hexgon {

static void FillDesc(MTLTextureDescriptor* mtl_desc,
                     const TextureDescriptor& desc) {}

TextureMTL::TextureMTL(const TextureDescriptor& desc, id<MTLTexture> texture)
    : Texture(desc), m_native_texture(texture) {}

TextureMTL::~TextureMTL() { [m_native_texture release]; }

Ref<TextureMTL> TextureMTL::CreateTexture(const TextureDescriptor& desc,
                                          id<MTLDevice> gpu) {
  MTLTextureDescriptor* mtl_desc = [MTLTextureDescriptor new];

  FillDesc(mtl_desc, desc);

  id<MTLTexture> texture = [gpu newTextureWithDescriptor:mtl_desc];

  [mtl_desc release];

  if (texture == nil) {
    return nullptr;
  }

  return CreateRef<TextureMTL>(desc, texture);
}

}
