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

#include "ColorMaterial.hpp"

using namespace hexgon;

struct BoxPushConstBlock {
  alignas(16) glm::vec3 eye;
};

class SimpleMesh : public Mesh {
 public:
  SimpleMesh(Geometry* geometry, Material* material) : Mesh(geometry, material) {}
  ~SimpleMesh() override = default;

 protected:
  void OnInit(GraphicsContext* ctx) override { m_obj_buffer = ctx->CreateUniformBuffer(sizeof(glm::mat4)); }

  void OnPrepareDraw() override {
    auto matrix = CalculateMatrix();

    m_obj_buffer->UploadData(&matrix, sizeof(glm::mat4), 0);

    std::vector<gpu::DescriptorBinding> bindings;
    bindings.emplace_back(gpu::DescriptorBinding{m_obj_buffer.get()});

    GetMaterial()->PrepareForDraw(bindings);

    UpdateDescriptorSet(1, bindings, 0);
  }

 protected:
  std::unique_ptr<gpu::UniformBuffer> m_obj_buffer;
};

class Simple3DLayer : public Layer {
 public:
  Simple3DLayer() : Layer("Simple3DLayer") {}
  ~Simple3DLayer() override = default;

 protected:
  void OnAttach() override {
    m_eye = glm::vec3(0.f, -1.f, 5.f);
    m_view_proj = glm::perspective(glm::radians(60.f), 4.f / 3.f, 0.001f, 1000.f) *
                  glm::lookAt(m_eye, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    m_global_buffer = GetGraphicsContext()->CreateUniformBuffer(sizeof(glm::mat4));

    m_obj_buffer = GetGraphicsContext()->CreateUniformBuffer(sizeof(glm::mat4));

    m_box_geometry = Geometry::MakeBox();

    m_box_geometry->InitBuffer(GetGraphicsContext());

    m_color_pipeline = example::ColorMaterial::CreatePipeline(GetGraphicsContext());
    m_color_material = std::make_unique<example::ColorMaterial>(glm::vec4(1.f, 0.f, 0.f, 1.f), m_color_pipeline.get());

    m_color_material->Init(GetGraphicsContext());

    m_box_mesh = std::make_unique<SimpleMesh>(m_box_geometry.get(), m_color_material.get());

    m_box_mesh->Init(GetGraphicsContext());

    m_box_mesh->SetPosition({0.4f, 0.f, 0.f});

    InitBuffers();
  }

  void OnUpdate(float tm) override {
    m_global_buffer->UploadData(&m_view_proj, sizeof(glm::mat4), 0);

    glm::mat4 model = glm::identity<glm::mat4>();
    m_obj_buffer->UploadData(&model, sizeof(glm::mat4), 0);

    auto cmd = GetGraphicsContext()->CurrentCommandBuffer();

    auto rotation = m_box_mesh->GetRotation();

    rotation.y += glm::radians(1.f);

    m_box_mesh->SetRotation(rotation);

    m_box_mesh->Bind(cmd);

    std::vector<gpu::DescriptorBinding> bindings;
    bindings.emplace_back(gpu::DescriptorBinding{m_global_buffer.get()});

    // global sets
    m_box_mesh->UpdateDescriptorSet(0, bindings, 0);

    // push constants
    BoxPushConstBlock block;
    block.eye = m_eye;
    m_box_mesh->UpdatePushConstant(0, sizeof(BoxPushConstBlock), &block);

    m_box_mesh->Draw(cmd);
  }

  void OnDetach() override {
    m_box_mesh.reset();
    m_color_material.reset();
    m_color_pipeline.reset();
    m_box_geometry.reset();
    m_global_buffer.reset();
  }

  void OnEvent(const Event* event) override {}

  void InitBuffers() {
    gpu::BufferLayout layout({
        gpu::BufferElement("pos", gpu::DataType::Float3, 0),
        gpu::BufferElement("color", gpu::DataType::Float4, 3 * sizeof(float)),
    });

    m_vertex_buffer = GetGraphicsContext()->CreateVertexBuffer(layout);

    std::vector<float> vertex_data{
        // x   y      z     r    g    b   a
        -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,  // point 1
        0.5f,  -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f,  // point 2
        -0.5f, 0.5f,  0.f, 0.f, 0.f, 1.f, 1.f, 0.f,  // point 2
        0.5f,  0.5f,  0.f, 1.f, 1.f, 0.f, 1.f, 0.f,  // point 3
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

 private:
  glm::vec3 m_eye;
  glm::mat4 m_view_proj;
  std::unique_ptr<Geometry> m_box_geometry;
  std::unique_ptr<gpu::Pipeline> m_color_pipeline;
  std::unique_ptr<Material> m_color_material;
  std::unique_ptr<Mesh> m_box_mesh;
  std::unique_ptr<gpu::UniformBuffer> m_global_buffer;
  std::unique_ptr<gpu::UniformBuffer> m_obj_buffer;
  std::unique_ptr<gpu::VertexBuffer> m_vertex_buffer;
  std::unique_ptr<gpu::IndexBuffer> m_index_buffer;
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  HEX_INFO("create app instance 0x{:x}", reinterpret_cast<uintptr_t>(app));

  app->GetWindow()->SetClearColor(glm::vec4{0.f, 0.f, 0.f, 0.f});

  app->PushLayer(std::make_shared<Simple3DLayer>());

  app->Run();

  return 0;
}
