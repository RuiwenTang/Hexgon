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

#include "ColorMaterial.hpp"

#include "material_shaders.hpp"

namespace example {

ColorMaterial::ColorMaterial(glm::vec4 const& color, hexgon::gpu::Pipeline* pipeline)
    : hexgon::Material(pipeline), m_color(color) {}

void ColorMaterial::Init(hexgon::GraphicsContext* ctx) { m_color_buffer = ctx->CreateUniformBuffer(sizeof(glm::vec4)); }

void ColorMaterial::OnPrepareForDraw(std::vector<hexgon::gpu::DescriptorBinding>& bindings) {
  m_color_buffer->UploadData(&m_color, sizeof(glm::vec4), 0);

  bindings.emplace_back(hexgon::gpu::DescriptorBinding{m_color_buffer.get()});
}

std::unique_ptr<hexgon::gpu::Pipeline> ColorMaterial::CreatePipeline(hexgon::GraphicsContext* ctx) {
  hexgon::gpu::PipelineInfo info;

  // sample count
  info.sample_count = ctx->GetSampleCount();

  info.shaders.emplace_back(hexgon::gpu::Shader(hexgon::gpu::Shader::Stage::Vertex,
                                                (const char*)color_material_vert_spv, color_material_vert_spv_size));
  info.shaders.emplace_back(hexgon::gpu::Shader(hexgon::gpu::Shader::Stage::Fragment,
                                                (const char*)color_material_frag_spv, color_material_frag_spv_size));
  info.vertex_binding.emplace_back(hexgon::gpu::VertexBinding{0, 8 * sizeof(float)});
  // pos
  info.attr_desc.emplace_back(hexgon::gpu::VertexAttributeDescriptor{0, 0, hexgon::gpu::DataType::Float3, 0});
  // normal
  info.attr_desc.emplace_back(
      hexgon::gpu::VertexAttributeDescriptor{0, 1, hexgon::gpu::DataType::Float4, 3 * sizeof(float)});

  // attachments
  info.color_attachment = ctx->ScreenColorAttachment();
  info.depth_attachment = ctx->ScreenDepthAttachment();

  // render pass
  info.render_pass = ctx->ScreenRenderPass();

  return ctx->CreatePipeline(info);
}

}  // namespace example