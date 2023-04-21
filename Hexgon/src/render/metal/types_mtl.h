#pragma once

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

#include <hexgon/render/format.hpp>
#include <hexgon/render/renderpass.hpp>
#include <hexgon/render/texture.hpp>

namespace Hexgon {
namespace MTL {

template <typename G, typename V>
struct Convertor {
  static V ToNative(G value);
  static G ToGPU(V value);
};

#define CONVERTOR(G, V)    \
  template <>              \
  struct Convertor<G, V> { \
    static V ToNative(G);  \
    static G ToGPU(V);     \
  }

CONVERTOR(Format, MTLPixelFormat);
CONVERTOR(AttachmentLoadOp, MTLLoadAction);
CONVERTOR(AttachmentStoreOp, MTLStoreAction);
CONVERTOR(TextureType, MTLTextureType);

}  // namespace MTL
}  // namespace Hexgon