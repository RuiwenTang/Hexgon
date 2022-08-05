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

#include <Hexgon/Hexgon.hpp>

#include "ShaderSource.hpp"

using namespace hexgon;

class SimpleLayer : public Layer {
 public:
  SimpleLayer() : Layer("SimpleLayer") {}

  ~SimpleLayer() override = default;

  void OnAttach() override {
    HEX_INFO("{} layer OnAttach", this->GetLayerName());
    HEX_INFO("current GraphicContext: 0x{:x}", reinterpret_cast<uintptr_t>(GetGraphicsContext()));

    InitPipeline();
    InitBuffers();
  }

  void OnDetach() override {
    HEX_INFO("{} layer OnDetach", this->GetLayerName());

    m_index_buffer.reset();
    m_vertex_buffer.reset();
    m_pipeline.reset();
    m_ubo_pipeline.reset();
  }

  void OnUpdate(float tm) override {
    auto cmd = GetGraphicsContext()->CurrentCommandBuffer();

    cmd->BindPipeline(m_pipeline.get());

    cmd->BindVertexBuffer(m_vertex_buffer.get(), 0);

    // cmd->Draw(3, 1, 0, 0);

    cmd->BindIndexBuffer(m_index_buffer.get());

    cmd->DrawIndexed(6, 1, 0, 0);
  }

  void OnEvent(const Event* event) override {
    HEX_INFO("{} layer OnEvent event: {}", this->GetLayerName(), event->GetName());
  }

 private:
  void InitPipeline() {
    gpu::PipelineInfo info;
    // sample count
    info.sample_count = GetGraphicsContext()->GetSampleCount();
    // vertex shader
    info.shaders.emplace_back(
        gpu::Shader(gpu::Shader::Stage::Vertex, (const char*)hello_triangle_vert_spv, hello_triangle_vert_spv_size));
    // fragment shader
    info.shaders.emplace_back(
        gpu::Shader(gpu::Shader::Stage::Fragment, (const char*)hello_triangle_frag_spv, hello_triangle_frag_spv_size));

    // one vertex array
    info.vertex_binding.emplace_back(gpu::VertexBinding{0, 7 * sizeof(float)});
    // one vertex buffer with two attributes
    // location 0 pos
    info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 0, gpu::DataType::Float3, 0});
    // location 1 pos
    info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 1, gpu::DataType::Float4, 3 * sizeof(float)});

    // attachments
    info.color_attachment = GetGraphicsContext()->ScreenColorAttachment();
    info.depth_attachment = GetGraphicsContext()->ScreenDepthAttachment();

    // render pass
    info.render_pass = GetGraphicsContext()->ScreenRenderPass();

    m_pipeline = GetGraphicsContext()->CreatePipeline(info);

    info.shaders[0] = gpu::Shader(gpu::Shader::Stage::Vertex, (const char*)hello_ubo_triangle_vert_spv,
                                  hello_ubo_triangle_vert_spv_size);
    info.shaders[1] = gpu::Shader(gpu::Shader::Stage::Fragment, (const char*)hello_ubo_triangle_frag_spv,
                                  hello_ubo_triangle_frag_spv_size);

    m_ubo_pipeline = GetGraphicsContext()->CreatePipeline(info);
  }

  void InitBuffers() {
    gpu::BufferLayout layout({
        gpu::BufferElement("pos", gpu::DataType::Float3, 0),
        gpu::BufferElement("color", gpu::DataType::Float4, 3 * sizeof(float)),
    });

    m_vertex_buffer = GetGraphicsContext()->CreateVertexBuffer(layout);

    std::vector<float> vertex_data{
        // x   y      z     r    g    b   a
        -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f,  // point 1
        0.5f,  -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f,  // point 2
        -0.5f, 0.5f,  0.f, 0.f, 0.f, 1.f, 1.f,  // point 2
        0.5f,  0.5f,  0.f, 1.f, 1.f, 0.f, 1.f,  // point 3
    };

    m_vertex_buffer->UploadData(vertex_data.data(), vertex_data.size() * sizeof(float));

    std::vector<uint32_t> index_data{
        0, 1, 2, 1, 2, 3,
    };

    m_index_buffer = GetGraphicsContext()->CreateIndexBuffer();

    m_index_buffer->UploadData(index_data.data(), index_data.size() * sizeof(uint32_t));
  }

 private:
  std::unique_ptr<gpu::Pipeline> m_pipeline;
  std::unique_ptr<gpu::Pipeline> m_ubo_pipeline;
  std::unique_ptr<gpu::VertexBuffer> m_vertex_buffer;
  std::unique_ptr<gpu::IndexBuffer> m_index_buffer;
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  HEX_INFO("create app instance 0x{:x}", reinterpret_cast<uintptr_t>(app));

  app->GetWindow()->SetClearColor(glm::vec4{1.f, 1.f, 1.f, 1.f});

  app->PushLayer(std::make_shared<SimpleLayer>());

  app->Run();

  return 0;
}
