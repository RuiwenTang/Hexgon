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

#ifndef ENGINE_INCLUDE_HEXGON_GPU_PIPELINE_HPP_
#define ENGINE_INCLUDE_HEXGON_GPU_PIPELINE_HPP_

#include <Hexgon/GPU/Formats.hpp>
#include <Hexgon/GPU/Shader.hpp>
#include <Hexgon/Macro.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace hexgon {

namespace gpu {

class RenderPass;
class UniformBuffer;

struct VertexBinding {
  uint32_t slot = 0;
  uint32_t stride = 0;
};

struct VertexAttributeDescriptor {
  uint32_t slot = 0;
  uint32_t location = 0;
  DataType type = DataType::None;
  uint32_t offset = 0;
};

struct BlendInfo {
  BlendFactor src = BlendFactor::SourceAlpha;
  BlendFactor dst = BlendFactor::OneMinusSourceAlpha;
  BlendOperation op = BlendOperation::Add;
};

struct ColorAttachmentDescriptor {
  bool blending = false;
  BlendInfo color = {};
  BlendInfo alpha = {};
};

struct DepthAttachmentDescriptor {
  CompareFunction compare = CompareFunction::Always;
  bool depth_writable = false;
};

struct PipelineInfo {
  std::vector<Shader> shaders = {};
  PrimitiveType primitive = PrimitiveType::Triangles;
  SampleCount sample_count = SampleCount::Count1;
  std::vector<VertexBinding> vertex_binding = {};
  std::vector<VertexAttributeDescriptor> attr_desc = {};
  std::vector<ColorAttachmentDescriptor> color_attachment = {};
  std::vector<DepthAttachmentDescriptor> depth_attachment = {};
  RenderPass* render_pass = nullptr;
};

// TODO support image texture
struct DescriptorBinding {
  UniformBuffer* ubo = nullptr;
};

class Pipeline {
 public:
  virtual ~Pipeline() = default;

  virtual void UpdateDescriptorSet(std::string const& name, std::vector<DescriptorBinding> const& bindings) = 0;
};

}  // namespace gpu

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GPU_PIPELINE_HPP_