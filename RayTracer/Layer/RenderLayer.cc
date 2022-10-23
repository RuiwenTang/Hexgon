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

#include "Layer/RenderLayer.hpp"

#include <future>

#include "texture_glsl.hpp"

using namespace hexgon;

uint32_t ToRGBA(glm::vec4 const& color) {
  uint8_t r = (uint8_t)(color.r * 255.0f);
  uint8_t g = (uint8_t)(color.g * 255.0f);
  uint8_t b = (uint8_t)(color.b * 255.0f);
  uint8_t a = (uint8_t)(color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

void RenderLayer::OnAttach() {
  InitPipeline();
  InitBuffers();
  InitImage();
}

void RenderLayer::OnDetach() {
  m_texture.reset();
  m_gpu_texture.reset();
  m_index_buffer.reset();
  m_vertex_buffer.reset();
  m_pipeline.reset();
}

void RenderLayer::OnUpdate(float tm) {
  UploadTextureIfNeed();

  auto cmd = GetGraphicsContext()->CurrentCommandBuffer();

  cmd->BindPipeline(m_pipeline.get());

  cmd->BindVertexBuffer(m_vertex_buffer.get(), 0);

  cmd->BindIndexBuffer(m_index_buffer.get());

  std::vector<gpu::DescriptorBinding> bindings;
  bindings.emplace_back(gpu::DescriptorBinding{0, m_gpu_texture.get()});
  m_pipeline->UpdateDescriptorSet(0, bindings);

  cmd->DrawIndexed(6, 1, 0, 0);
}

void RenderLayer::OnRender() {
  std::lock_guard guad{m_mutex};
  if (m_rendering || m_image_dirty) {
    return;
  }

  std::thread render_thread{[&]() {
    {
      std::lock_guard begin_guard{m_mutex};
      m_rendering = true;
    }

    HEX_INFO("Render Thread begin rendering!!");
    DoRender();
    HEX_INFO("Render Thread end rendering!!");
    {
      std::lock_guard end_guard{m_mutex};
      m_rendering = false;
      m_image_dirty = true;
    }
  }};

  render_thread.detach();
}

void RenderLayer::SetRenderer(Renderer* renderer) {
  m_renderer = renderer;
  m_renderer->SetLayer(this);
}

void RenderLayer::InitPipeline() {
  gpu::PipelineInfo info;
  // sample count
  info.sample_count = GetGraphicsContext()->GetSampleCount();
  // vertex shader
  info.shaders.emplace_back(
      gpu::Shader(gpu::Shader::Stage::Vertex, (const char*)texture_vert_spv, texture_vert_spv_size));
  // fragment shader
  info.shaders.emplace_back(
      gpu::Shader(gpu::Shader::Stage::Fragment, (const char*)texture_frag_spv, texture_frag_spv_size));

  // one vertex array
  info.vertex_binding.emplace_back(gpu::VertexBinding{0, 5 * sizeof(float)});
  // one vertex buffer with two attributes
  // location 0 pos
  info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 0, gpu::DataType::Float3, 0});
  // location 1 pos
  info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 1, gpu::DataType::Float2, 3 * sizeof(float)});

  // attachments
  info.color_attachment = GetGraphicsContext()->ScreenColorAttachment();
  info.depth_attachment = GetGraphicsContext()->ScreenDepthAttachment();

  // render pass
  info.render_pass = GetGraphicsContext()->ScreenRenderPass();

  m_pipeline = GetGraphicsContext()->CreatePipeline(info);
}

void RenderLayer::InitBuffers() {
  gpu::BufferLayout layout({
      gpu::BufferElement("pos", gpu::DataType::Float3, 0),
      gpu::BufferElement("uv", gpu::DataType::Float2, 3 * sizeof(float)),
  });

  m_vertex_buffer = GetGraphicsContext()->CreateVertexBuffer(layout);

  std::vector<float> vertex_data{
      // x   y      z     u    v
      -1.f, 1.f,  0.f, 0.f, 0.f,  // point 1
      1.0f, 1.f,  0.f, 1.f, 0.f,  // point 2
      -1.f, -1.f, 0.f, 0.f, 1.f,  // point 2
      1.f,  -1.f, 0.f, 1.f, 1.f,  // point 3
  };

  m_vertex_buffer->Resize(vertex_data.size() * sizeof(float));

  m_vertex_buffer->UploadData(vertex_data.data(), vertex_data.size() * sizeof(float), 0);

  std::vector<uint32_t> index_data{
      0, 1, 2, 1, 2, 3,
  };

  m_index_buffer = GetGraphicsContext()->CreateIndexBuffer();

  m_index_buffer->Resize(index_data.size() * sizeof(uint32_t));

  m_index_buffer->UploadData(index_data.data(), index_data.size() * sizeof(uint32_t), 0);
}

void RenderLayer::InitImage() {
  uint32_t width = GetApplication()->GetWindow()->GetWidth();
  uint32_t height = GetApplication()->GetWindow()->GetHeight();

  m_texture = hexgon::io::Image::CreateEmpty(width, height);

  m_gpu_texture = GetGraphicsContext()->CreateTexture(m_texture->GetFormat(), hexgon::gpu::TextureUsage::ShaderRead,
                                                      hexgon::gpu::TextureType::kImageTexture);

  m_gpu_texture->Resize(m_texture->GetWidth(), m_texture->GetHeight());

  if (m_renderer) {
    m_renderer->InitViewPort(m_texture->GetWidth(), m_texture->GetHeight());
  }
}

void RenderLayer::UploadTexture() {
  m_gpu_texture->UploadData(0, 0, m_texture->GetWidth(), m_texture->GetHeight(), m_texture->GetRawData());
}

void RenderLayer::DoRender() {
  if (m_renderer) {
    m_renderer->DoRender(m_texture.get());
  }
}

void RenderLayer::UploadTextureIfNeed() {
  std::lock_guard guard{m_mutex};

  if (m_rendering) {
    return;
  }

  if (!m_image_dirty) {
    return;
  }

  UploadTexture();

  m_image_dirty = false;
}

uint32_t RenderLayer::Renderer::ToRGBA(glm::vec4 const& pixel) { return ::ToRGBA(pixel); }
