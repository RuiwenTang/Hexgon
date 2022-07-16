/*
 *   Copyright (c) 2022 RuiwenTang
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#ifndef ENGINE_INCLUDE_HEXGON_GPU_FORMATS_HPP
#define ENGINE_INCLUDE_HEXGON_GPU_FORMATS_HPP

#include <cstdint>

namespace hexgon {
namespace gpu {

enum class PixelFormat {
  Unknown,
  A8UNormInt,
  R8G8B8A8UNormInt,
  R8G8B8A8UNormIntSRGB,
  B8G8R8A8UNormInt,
  B8G8R8A8UNormIntSRGB,
  S8Uint,
};

enum class DataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
};

enum class BlendFactor {
  Zero,
  One,
  SourceColor,
  OneMinusSourceColor,
  SourceAlpha,
  OneMinusSourceAlpha,
  DestinationColor,
  OneMinusDestinationColor,
  DestinationAlpha,
  OneMinusDestinationAlpha,
  SourceAlphaSaturated,
  BlendColor,
  OneMinusBlendColor,
  BlendAlpha,
  OneMinusBlendAlpha,
};

enum class BlendOperation {
  Add,
  Subtract,
  ReverseSubtract,
};

enum class LoadAction {
  DontCare,
  Load,
  Clear,
};

enum class StoreAction {
  DontCare,
  Store,
  MultisampleResolve,
};

enum class SampleCount {
  Count1,
  Count4,
  Count8,
  Count16,
  Count32,
  Count64,
};

enum class TextureUsage : uint64_t {
  Unknown = 0,
  ShaderRead = 1 << 0,
  ShaderWrite = 1 << 1,
  RenderTarget = 1 << 2,
};

enum class CullMode {
  None,
  FrontFace,
  BackFace,
};

enum class PrimitiveType {
  Triangles,
  TriangleStrip,
  Line,
  LineStrip,
  Point,
};

}  // namespace gpu
}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GPU_FORMATS_HPP