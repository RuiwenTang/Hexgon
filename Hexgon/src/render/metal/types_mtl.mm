#include "src/render/metal/types_mtl.h"

#include <Metal/Metal.h>
#include <hexgon/core/platform.hpp>

namespace Hexgon {
namespace MTL {

Format Convertor<Format, MTLPixelFormat>::ToGPU(MTLPixelFormat value) {
  switch (value) {
    case MTLPixelFormatInvalid:
      return Format::Undefined;

    /* --- Alpha channel color formats --- */
    case MTLPixelFormatA8Unorm:
      return Format::A8UNorm;

    /* --- Red channel color formats --- */
    case MTLPixelFormatR8Unorm:
      return Format::R8UNorm;
    case MTLPixelFormatR8Snorm:
      return Format::R8SNorm;
    case MTLPixelFormatR8Uint:
      return Format::R8UInt;
    case MTLPixelFormatR8Sint:
      return Format::R8SInt;

    case MTLPixelFormatR16Unorm:
      return Format::R16UNorm;
    case MTLPixelFormatR16Snorm:
      return Format::R16SNorm;
    case MTLPixelFormatR16Uint:
      return Format::R16UInt;
    case MTLPixelFormatR16Sint:
      return Format::R16SInt;
    case MTLPixelFormatR16Float:
      return Format::R16Float;

    case MTLPixelFormatR32Uint:
      return Format::R32UInt;
    case MTLPixelFormatR32Sint:
      return Format::R32SInt;
    case MTLPixelFormatR32Float:
      return Format::R32Float;

    /* --- RG color formats --- */
    case MTLPixelFormatRG8Unorm:
      return Format::RG8UNorm;
    case MTLPixelFormatRG8Snorm:
      return Format::RG8SNorm;
    case MTLPixelFormatRG8Uint:
      return Format::RG8UInt;
    case MTLPixelFormatRG8Sint:
      return Format::RG8SInt;

    case MTLPixelFormatRG16Unorm:
      return Format::RG16UNorm;
    case MTLPixelFormatRG16Snorm:
      return Format::RG16SNorm;
    case MTLPixelFormatRG16Uint:
      return Format::RG16UInt;
    case MTLPixelFormatRG16Sint:
      return Format::RG16SInt;
    case MTLPixelFormatRG16Float:
      return Format::RG16Float;

    case MTLPixelFormatRG32Uint:
      return Format::RG32UInt;
    case MTLPixelFormatRG32Sint:
      return Format::RG32SInt;
    case MTLPixelFormatRG32Float:
      return Format::RG32Float;

    /* --- RGBA color formats --- */
    case MTLPixelFormatRGBA8Unorm:
      return Format::RGBA8UNorm;
    case MTLPixelFormatRGBA8Snorm:
      return Format::RGBA8SNorm;
    case MTLPixelFormatRGBA8Uint:
      return Format::RGBA8UInt;
    case MTLPixelFormatRGBA8Sint:
      return Format::RGBA8SInt;

    case MTLPixelFormatRGBA16Unorm:
      return Format::RGBA16UNorm;
    case MTLPixelFormatRGBA16Snorm:
      return Format::RGBA16SNorm;
    case MTLPixelFormatRGBA16Uint:
      return Format::RGBA16UInt;
    case MTLPixelFormatRGBA16Sint:
      return Format::RGBA16SInt;
    case MTLPixelFormatRGBA16Float:
      return Format::RGBA16Float;

    case MTLPixelFormatRGBA32Uint:
      return Format::RGBA32UInt;
    case MTLPixelFormatRGBA32Sint:
      return Format::RGBA32SInt;
    case MTLPixelFormatRGBA32Float:
      return Format::RGBA32Float;

    /* --- BGRA color formats --- */
    case MTLPixelFormatBGRA8Unorm:
      return Format::BGRA8UNorm;
    case MTLPixelFormatBGRA8Unorm_sRGB:
      return Format::BGRA8UNorm_sRGB;

    /* --- Packed formats --- */
    case MTLPixelFormatRGB10A2Unorm:
      return Format::RGB10A2UNorm;
    case MTLPixelFormatRGB10A2Uint:
      return Format::RGB10A2UInt;
    case MTLPixelFormatRG11B10Float:
      return Format::RG11B10Float;
    case MTLPixelFormatRGB9E5Float:
      return Format::RGB9E5Float;

      /* --- Depth-stencil formats --- */
#ifndef HEX_PLATFORM_IOS
    case MTLPixelFormatDepth16Unorm:
      return Format::D16UNorm;
#endif  // HEX_PLATFORM_IOS
    case MTLPixelFormatDepth32Float:
      return Format::D32Float;
#ifndef HEX_PLATFORM_IOS
    case MTLPixelFormatDepth24Unorm_Stencil8:
      return Format::D24UNormS8UInt;
#endif  // HEX_PLATFORM_IOS
    case MTLPixelFormatDepth32Float_Stencil8:
      return Format::D32FloatS8X24UInt;

#ifndef HEX_PLATFORM_IOS
    /* --- Compressed color formats --- */
    case MTLPixelFormatBC1_RGBA:
      return Format::BC1UNorm;
    case MTLPixelFormatBC1_RGBA_sRGB:
      return Format::BC1UNorm_sRGB;
    case MTLPixelFormatBC2_RGBA:
      return Format::BC2UNorm;
    case MTLPixelFormatBC2_RGBA_sRGB:
      return Format::BC2UNorm_sRGB;
    case MTLPixelFormatBC3_RGBA:
      return Format::BC3UNorm;
    case MTLPixelFormatBC3_RGBA_sRGB:
      return Format::BC3UNorm_sRGB;
    case MTLPixelFormatBC4_RUnorm:
      return Format::BC4UNorm;
    case MTLPixelFormatBC4_RSnorm:
      return Format::BC4SNorm;
    case MTLPixelFormatBC5_RGUnorm:
      return Format::BC5UNorm;
    case MTLPixelFormatBC5_RGSnorm:
      return Format::BC5SNorm;
#endif  // HEX_PLATFORM_IOS
    default:
      break;
  }
  return Format::Undefined;
}

MTLPixelFormat Convertor<Format, MTLPixelFormat>::ToNative(Format value) {
  switch (value) {
    case Format::Undefined:
      return MTLPixelFormatInvalid;

    /* --- Alpha channel color formats --- */
    case Format::A8UNorm:
      return MTLPixelFormatA8Unorm;

    /* --- Red channel color formats --- */
    case Format::R8UNorm:
      return MTLPixelFormatR8Unorm;
    case Format::R8SNorm:
      return MTLPixelFormatR8Snorm;
    case Format::R8UInt:
      return MTLPixelFormatR8Uint;
    case Format::R8SInt:
      return MTLPixelFormatR8Sint;

    case Format::R16UNorm:
      return MTLPixelFormatR16Unorm;
    case Format::R16SNorm:
      return MTLPixelFormatR16Snorm;
    case Format::R16UInt:
      return MTLPixelFormatR16Uint;
    case Format::R16SInt:
      return MTLPixelFormatR16Sint;
    case Format::R16Float:
      return MTLPixelFormatR16Float;

    case Format::R32UInt:
      return MTLPixelFormatR32Uint;
    case Format::R32SInt:
      return MTLPixelFormatR32Sint;
    case Format::R32Float:
      return MTLPixelFormatR32Float;

    case Format::R64Float:
      break;

    /* --- RG color formats --- */
    case Format::RG8UNorm:
      return MTLPixelFormatRG8Unorm;
    case Format::RG8SNorm:
      return MTLPixelFormatRG8Snorm;
    case Format::RG8UInt:
      return MTLPixelFormatRG8Uint;
    case Format::RG8SInt:
      return MTLPixelFormatRG8Sint;

    case Format::RG16UNorm:
      return MTLPixelFormatRG16Unorm;
    case Format::RG16SNorm:
      return MTLPixelFormatRG16Snorm;
    case Format::RG16UInt:
      return MTLPixelFormatRG16Uint;
    case Format::RG16SInt:
      return MTLPixelFormatRG16Sint;
    case Format::RG16Float:
      return MTLPixelFormatRG16Float;

    case Format::RG32UInt:
      return MTLPixelFormatRG32Uint;
    case Format::RG32SInt:
      return MTLPixelFormatRG32Sint;
    case Format::RG32Float:
      return MTLPixelFormatRG32Float;

    case Format::RG64Float:
      break;

    /* --- RGB color formats --- */
    case Format::RGB8UNorm:
      break;
    case Format::RGB8UNorm_sRGB:
      break;
    case Format::RGB8SNorm:
      break;
    case Format::RGB8UInt:
      break;
    case Format::RGB8SInt:
      break;

    case Format::RGB16UNorm:
      break;
    case Format::RGB16SNorm:
      break;
    case Format::RGB16UInt:
      break;
    case Format::RGB16SInt:
      break;
    case Format::RGB16Float:
      break;

    case Format::RGB32UInt:
      break;
    case Format::RGB32SInt:
      break;
    case Format::RGB32Float:
      break;

    case Format::RGB64Float:
      break;

    /* --- RGBA color formats --- */
    case Format::RGBA8UNorm:
      return MTLPixelFormatRGBA8Unorm;
    case Format::RGBA8UNorm_sRGB:
      return MTLPixelFormatRGBA8Unorm_sRGB;
    case Format::RGBA8SNorm:
      return MTLPixelFormatRGBA8Snorm;
    case Format::RGBA8UInt:
      return MTLPixelFormatRGBA8Uint;
    case Format::RGBA8SInt:
      return MTLPixelFormatRGBA8Sint;

    case Format::RGBA16UNorm:
      return MTLPixelFormatRGBA16Unorm;
    case Format::RGBA16SNorm:
      return MTLPixelFormatRGBA16Snorm;
    case Format::RGBA16UInt:
      return MTLPixelFormatRGBA16Uint;
    case Format::RGBA16SInt:
      return MTLPixelFormatRGBA16Sint;
    case Format::RGBA16Float:
      return MTLPixelFormatRGBA16Float;

    case Format::RGBA32UInt:
      return MTLPixelFormatRGBA32Uint;
    case Format::RGBA32SInt:
      return MTLPixelFormatRGBA32Sint;
    case Format::RGBA32Float:
      return MTLPixelFormatRGBA32Float;

    case Format::RGBA64Float:
      break;

    /* --- BGRA color formats --- */
    case Format::BGRA8UNorm:
      return MTLPixelFormatBGRA8Unorm;
    case Format::BGRA8UNorm_sRGB:
      return MTLPixelFormatBGRA8Unorm_sRGB;
    case Format::BGRA8SNorm:
      break;
    case Format::BGRA8UInt:
      break;
    case Format::BGRA8SInt:
      break;

    /* --- Packed formats --- */
    case Format::RGB10A2UNorm:
      return MTLPixelFormatRGB10A2Unorm;
    case Format::RGB10A2UInt:
      return MTLPixelFormatRGB10A2Uint;
    case Format::RG11B10Float:
      return MTLPixelFormatRG11B10Float;
    case Format::RGB9E5Float:
      return MTLPixelFormatRGB9E5Float;

/* --- Depth-stencil formats --- */
#ifndef HEX_PLATFORM_IOS
    case Format::D16UNorm:
      return MTLPixelFormatDepth16Unorm;
#endif
    case Format::D32Float:
      return MTLPixelFormatDepth32Float;
#ifndef HEX_PLATFORM_IOS
    case Format::D24UNormS8UInt:
      return MTLPixelFormatDepth24Unorm_Stencil8;
#endif
    case Format::D32FloatS8X24UInt:
      return MTLPixelFormatDepth32Float_Stencil8;

#ifndef HEX_PLATFORM_IOS
    /* --- Compressed color formats --- */
    case Format::BC1UNorm:
      return MTLPixelFormatBC1_RGBA;
    case Format::BC1UNorm_sRGB:
      return MTLPixelFormatBC1_RGBA_sRGB;
    case Format::BC2UNorm:
      return MTLPixelFormatBC2_RGBA;
    case Format::BC2UNorm_sRGB:
      return MTLPixelFormatBC2_RGBA_sRGB;
    case Format::BC3UNorm:
      return MTLPixelFormatBC3_RGBA;
    case Format::BC3UNorm_sRGB:
      return MTLPixelFormatBC3_RGBA_sRGB;
    case Format::BC4UNorm:
      return MTLPixelFormatBC4_RUnorm;
    case Format::BC4SNorm:
      return MTLPixelFormatBC4_RSnorm;
    case Format::BC5UNorm:
      return MTLPixelFormatBC5_RGUnorm;
    case Format::BC5SNorm:
      return MTLPixelFormatBC5_RGSnorm;
#endif

    default:
      break;
  }
  return MTLPixelFormatInvalid;
}

MTLLoadAction Convertor<AttachmentLoadOp, MTLLoadAction>::ToNative(
    AttachmentLoadOp value) {
  switch (value) {
    case AttachmentLoadOp::kLoad:
      return MTLLoadActionLoad;
    case AttachmentLoadOp::kClear:
      return MTLLoadActionClear;
    case AttachmentLoadOp::kNotCare:
      return MTLLoadActionDontCare;
    default:
      return MTLLoadActionDontCare;
  }
}

AttachmentLoadOp Convertor<AttachmentLoadOp, MTLLoadAction>::ToGPU(
    MTLLoadAction value) {
  switch (value) {
    case MTLLoadActionLoad:
      return AttachmentLoadOp::kLoad;
    case MTLLoadActionClear:
      return AttachmentLoadOp::kClear;
    case MTLLoadActionDontCare:
      return AttachmentLoadOp::kNotCare;
    default:
      return AttachmentLoadOp::kNotCare;
  }
}

MTLStoreAction Convertor<AttachmentStoreOp, MTLStoreAction>::ToNative(
    AttachmentStoreOp value) {
  switch (value) {
    case AttachmentStoreOp::kStore:
      return MTLStoreActionStore;
    case AttachmentStoreOp::kNotCare:
      return MTLStoreActionDontCare;
    default:
      return MTLStoreActionDontCare;
  }
}

AttachmentStoreOp Convertor<AttachmentStoreOp, MTLStoreAction>::ToGPU(
    MTLStoreAction value) {
  switch (value) {
    case MTLStoreActionStore:
      return AttachmentStoreOp::kStore;
    case MTLStoreActionDontCare:
      return AttachmentStoreOp::kNotCare;
    default:
      return AttachmentStoreOp::kNotCare;
  }
}

}
}
