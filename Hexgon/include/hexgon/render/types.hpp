#pragma once

namespace Hexgon {

enum class ResourceType {
  kUndefined,
  kBuffer,
  kTexture,
  kSampler,
};

enum BindFlags {
  kUndefined = 0,
  kVertexBuffer = (1 << 0),
  kIndexBuffer = (1 << 1),
  kConstantBuffer = (1 << 2),
  kStreamOutputBuffer = (1 << 3),
  kIndirectBuffer = (1 << 4),
  kSampled = (1 << 5),
  kStorage = (1 << 6),
  kColorAttachment = (1 << 7),
  kDepthStencilAttachment = (1 << 8),
  kCombinedSampler = (1 << 9),
  kCopySrc = (1 << 10),
  kCopyDst = (1 << 11),
};

}  // namespace Hexgon