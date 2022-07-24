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

#include "GPU/VK/Formats.hpp"

namespace hexgon::gpu::vk {

VkSampleCountFlagBits Convertor<gpu::SampleCount, VkSampleCountFlagBits>::ToVulkan(gpu::SampleCount value) {
  switch (value) {
    case gpu::SampleCount::Count1:
      return VK_SAMPLE_COUNT_1_BIT;
    case gpu::SampleCount::Count4:
      return VK_SAMPLE_COUNT_4_BIT;
    case gpu::SampleCount::Count8:
      return VK_SAMPLE_COUNT_8_BIT;
    case gpu::SampleCount::Count16:
      return VK_SAMPLE_COUNT_16_BIT;
    case gpu::SampleCount::Count32:
      return VK_SAMPLE_COUNT_32_BIT;
    case gpu::SampleCount::Count64:
      return VK_SAMPLE_COUNT_64_BIT;
    default:
      return VK_SAMPLE_COUNT_1_BIT;
  }
}

gpu::SampleCount Convertor<gpu::SampleCount, VkSampleCountFlagBits>::ToGPU(VkSampleCountFlagBits value) {
  switch (value) {
    case VK_SAMPLE_COUNT_1_BIT:
      return gpu::SampleCount::Count1;
    case VK_SAMPLE_COUNT_4_BIT:
      return gpu::SampleCount::Count4;
    case VK_SAMPLE_COUNT_8_BIT:
      return gpu::SampleCount::Count8;
    case VK_SAMPLE_COUNT_16_BIT:
      return gpu::SampleCount::Count16;
    case VK_SAMPLE_COUNT_32_BIT:
      return gpu::SampleCount::Count32;
    case VK_SAMPLE_COUNT_64_BIT:
      return gpu::SampleCount::Count64;
    default:
      return gpu::SampleCount::Count1;
  }
}

gpu::Shader::Stage Convertor<gpu::Shader::Stage, VkShaderStageFlagBits>::ToGPU(VkShaderStageFlagBits value) {
  switch (value) {
    case VK_SHADER_STAGE_VERTEX_BIT:
      return gpu::Shader::Stage::Vertex;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
      return gpu::Shader::Stage::Fragment;
    case VK_SHADER_STAGE_COMPUTE_BIT:
      return gpu::Shader::Stage::Compute;
    case VK_SHADER_STAGE_GEOMETRY_BIT:
      return gpu::Shader::Stage::Geometry;
    default:
      // TODO support other vulkan shader stage
      return gpu::Shader::Stage::Unknown;
  }
}

VkShaderStageFlagBits Convertor<gpu::Shader::Stage, VkShaderStageFlagBits>::ToVulkan(gpu::Shader::Stage value) {
  switch (value) {
    case gpu::Shader::Stage::Vertex:
      return VK_SHADER_STAGE_VERTEX_BIT;
    case gpu::Shader::Stage::Fragment:
      return VK_SHADER_STAGE_FRAGMENT_BIT;
    case gpu::Shader::Stage::Geometry:
      return VK_SHADER_STAGE_GEOMETRY_BIT;
    case gpu::Shader::Stage::Compute:
      return VK_SHADER_STAGE_COMPUTE_BIT;
    default:
      return VK_SHADER_STAGE_ALL_GRAPHICS;
  }
}

gpu::DataType Convertor<gpu::DataType, VkFormat>::ToGPU(VkFormat value) {
  switch (value) {
    case VK_FORMAT_R32G32B32A32_SFLOAT:
      return gpu::DataType::Float4;
    case VK_FORMAT_R32G32B32_SFLOAT:
      return gpu::DataType::Float3;
    case VK_FORMAT_R32G32_SFLOAT:
      return gpu::DataType::Float2;
    case VK_FORMAT_R32_SFLOAT:
      return gpu::DataType::Float;
    case VK_FORMAT_R32G32B32A32_SINT:
      return gpu::DataType::Int4;
    case VK_FORMAT_R32G32B32_SINT:
      return gpu::DataType::Int3;
    case VK_FORMAT_R32G32_SINT:
      return gpu::DataType::Int2;
    case VK_FORMAT_R32_SINT:
      return gpu::DataType::Int;
    default:
      return gpu::DataType::None;
  }
}

VkFormat Convertor<gpu::DataType, VkFormat>::ToVulkan(gpu::DataType value) {
  switch (value) {
    case gpu::DataType::Float4:
      return VK_FORMAT_R32G32B32A32_SFLOAT;
    case gpu::DataType::Float3:
      return VK_FORMAT_R32G32B32_SFLOAT;
    case gpu::DataType::Float2:
      return VK_FORMAT_R32G32_SFLOAT;
    case gpu::DataType::Float:
      return VK_FORMAT_R32_SFLOAT;
    case gpu::DataType::Int4:
      return VK_FORMAT_R32G32B32A32_SINT;
    case gpu::DataType::Int3:
      return VK_FORMAT_R32G32B32_SINT;
    case gpu::DataType::Int2:
      return VK_FORMAT_R32G32_SINT;
    case gpu::DataType::Int:
      return VK_FORMAT_R32_SINT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

gpu::PrimitiveType Convertor<gpu::PrimitiveType, VkPrimitiveTopology>::ToGPU(VkPrimitiveTopology value) {
  switch (value) {
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
      return gpu::PrimitiveType::Triangles;
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
      return gpu::PrimitiveType::TriangleStrip;
    case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
      return gpu::PrimitiveType::Line;
    case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
      return gpu::PrimitiveType::LineStrip;
    default:
      return gpu::PrimitiveType::Point;
  }
}

VkPrimitiveTopology Convertor<gpu::PrimitiveType, VkPrimitiveTopology>::ToVulkan(gpu::PrimitiveType value) {
  switch (value) {
    case gpu::PrimitiveType::Triangles:
      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case gpu::PrimitiveType::TriangleStrip:
      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case gpu::PrimitiveType::Line:
      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case gpu::PrimitiveType::LineStrip:
      return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case gpu::PrimitiveType::Point:
      return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
  }
}

gpu::PixelFormat Convertor<gpu::PixelFormat, VkFormat>::ToGPU(VkFormat value) {
  switch (value) {
    case VK_FORMAT_R8G8B8A8_UNORM:
      return gpu::PixelFormat::R8G8B8A8UNormInt;
    case VK_FORMAT_R8G8B8A8_SRGB:
      return gpu::PixelFormat::B8G8R8A8UNormIntSRGB;
    case VK_FORMAT_R8_UNORM:
      return gpu::PixelFormat::A8UNormInt;
    case VK_FORMAT_B8G8R8A8_UNORM:
      return gpu::PixelFormat::B8G8R8A8UNormInt;
    case VK_FORMAT_B8G8R8A8_SRGB:
      return gpu::PixelFormat::B8G8R8A8UNormIntSRGB;
    case VK_FORMAT_R8_SINT:
      return gpu::PixelFormat::S8Uint;
    default:
      return gpu::PixelFormat::Unknown;
  }
}

VkFormat Convertor<gpu::PixelFormat, VkFormat>::ToVulkan(gpu::PixelFormat value) {
  switch (value) {
    case gpu::PixelFormat::R8G8B8A8UNormIntSRGB:
      return VK_FORMAT_R8G8B8A8_SRGB;
    case gpu::PixelFormat::R8G8B8A8UNormInt:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case gpu::PixelFormat::B8G8R8A8UNormIntSRGB:
      return VK_FORMAT_B8G8R8A8_SRGB;
    case gpu::PixelFormat::B8G8R8A8UNormInt:
      return VK_FORMAT_B8G8R8A8_UNORM;
    case gpu::PixelFormat::A8UNormInt:
      return VK_FORMAT_R8_UNORM;
    case gpu::PixelFormat::S8Uint:
      return VK_FORMAT_R8_SRGB;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

gpu::BlendFactor Convertor<gpu::BlendFactor, VkBlendFactor>::ToGPU(VkBlendFactor value) {
  switch (value) {
    case VK_BLEND_FACTOR_ZERO:
      return gpu::BlendFactor::Zero;
    case VK_BLEND_FACTOR_ONE:
      return gpu::BlendFactor::One;
    case VK_BLEND_FACTOR_SRC_COLOR:
      return gpu::BlendFactor::SourceColor;
    case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
      return gpu::BlendFactor::OneMinusSourceColor;
    case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
      return gpu::BlendFactor::OneMinusSourceAlpha;
    case VK_BLEND_FACTOR_DST_COLOR:
      return gpu::BlendFactor::DestinationColor;
    case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR:
      return gpu::BlendFactor::OneMinusDestinationColor;
    case VK_BLEND_FACTOR_DST_ALPHA:
      return gpu::BlendFactor::DestinationAlpha;
    case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
      return gpu::BlendFactor::OneMinusDestinationAlpha;
    default:
      return gpu::BlendFactor::Zero;
  }
}

VkBlendFactor Convertor<gpu::BlendFactor, VkBlendFactor>::ToVulkan(gpu::BlendFactor value) {
  switch (value) {
    case gpu::BlendFactor::Zero:
      return VK_BLEND_FACTOR_ZERO;
    case gpu::BlendFactor::One:
      return VK_BLEND_FACTOR_ONE;
    case gpu::BlendFactor::SourceColor:
      return VK_BLEND_FACTOR_SRC_COLOR;
    case gpu::BlendFactor::OneMinusSourceColor:
      return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case gpu::BlendFactor::SourceAlpha:
      return VK_BLEND_FACTOR_SRC_ALPHA;
    case gpu::BlendFactor::OneMinusSourceAlpha:
      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case gpu::BlendFactor::DestinationColor:
      return VK_BLEND_FACTOR_DST_COLOR;
    case gpu::BlendFactor::OneMinusDestinationColor:
      return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case gpu::BlendFactor::DestinationAlpha:
      return VK_BLEND_FACTOR_DST_ALPHA;
    case gpu::BlendFactor::OneMinusDestinationAlpha:
      return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
  }
}

gpu::BlendOperation Convertor<gpu::BlendOperation, VkBlendOp>::ToGPU(VkBlendOp value) {
  switch (value) {
    case VK_BLEND_OP_ADD:
      return gpu::BlendOperation::Add;
    case VK_BLEND_OP_SUBTRACT:
      return gpu::BlendOperation::Subtract;
    case VK_BLEND_OP_REVERSE_SUBTRACT:
      return gpu::BlendOperation::ReverseSubtract;
    default:
      return gpu::BlendOperation::Add;
  }
}

VkBlendOp Convertor<gpu::BlendOperation, VkBlendOp>::ToVulkan(gpu::BlendOperation value) {
  switch (value) {
    case gpu::BlendOperation::Add:
      return VK_BLEND_OP_ADD;
    case gpu::BlendOperation::Subtract:
      return VK_BLEND_OP_SUBTRACT;
    case gpu::BlendOperation::ReverseSubtract:
      return VK_BLEND_OP_REVERSE_SUBTRACT;
  }
}

gpu::CompareFunction Convertor<gpu::CompareFunction, VkCompareOp>::ToGPU(VkCompareOp value) {
  switch (value) {
    case VK_COMPARE_OP_NEVER:
      return gpu::CompareFunction::Never;
    case VK_COMPARE_OP_ALWAYS:
      return gpu::CompareFunction::Always;
    case VK_COMPARE_OP_EQUAL:
      return gpu::CompareFunction::Equal;
    case VK_COMPARE_OP_LESS_OR_EQUAL:
      return gpu::CompareFunction::LessEqual;
    case VK_COMPARE_OP_GREATER:
      return gpu::CompareFunction::Greater;
    case VK_COMPARE_OP_NOT_EQUAL:
      return gpu::CompareFunction::NotEqual;
    case VK_COMPARE_OP_GREATER_OR_EQUAL:
      return gpu::CompareFunction::GreaterEqual;
    default:
      return gpu::CompareFunction::Always;
  }
}

VkCompareOp Convertor<gpu::CompareFunction, VkCompareOp>::ToVulkan(gpu::CompareFunction value) {
  switch (value) {
    case gpu::CompareFunction::Never:
      return VK_COMPARE_OP_NEVER;
    case gpu::CompareFunction::Always:
      return VK_COMPARE_OP_ALWAYS;
    case gpu::CompareFunction::Less:
      return VK_COMPARE_OP_LESS;
    case gpu::CompareFunction::Equal:
      return VK_COMPARE_OP_EQUAL;
    case gpu::CompareFunction::LessEqual:
      return VK_COMPARE_OP_LESS_OR_EQUAL;
    case gpu::CompareFunction::Greater:
      return VK_COMPARE_OP_GREATER;
    case gpu::CompareFunction::NotEqual:
      return VK_COMPARE_OP_NOT_EQUAL;
    case gpu::CompareFunction::GreaterEqual:
      return VK_COMPARE_OP_GREATER_OR_EQUAL;
  }
}

}  // namespace hexgon::gpu::vk