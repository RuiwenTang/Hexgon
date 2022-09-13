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
    bindings.emplace_back(gpu::DescriptorBinding{0, m_obj_buffer.get()});

    GetMaterial()->PrepareForDraw(bindings);

    UpdateDescriptorSet(1, bindings);
  }

 protected:
  std::unique_ptr<gpu::UniformBuffer> m_obj_buffer;
};

class ControlLayer : public ImguiLayer {
 public:
  ControlLayer() = default;
  ~ControlLayer() override = default;

  float CurrentDegreeX() const { return m_degree_x; }
  float CurrentDegreeY() const { return m_degree_y; }
  float CurrentDegreeZ() const { return m_degree_z; }

 protected:
  void OnImguiInit() override {
    auto scale = GetApplication()->GetWindow()->GetDisplayScale();
    // Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config{};
    config.OversampleH = scale.y;
    config.OversampleV = scale.x;
    io.Fonts->AddFontFromFileTTF(RESOURCES_DIR "/Fonts/DroidSans.ttf", 16.f, &config);
  }

  void OnDrawImgui(float tm) override {
    ImGui::Begin("Control Panel");
    ImGui::SliderFloat("Rotate degree axis-x", &m_degree_x, 0.f, 360.f);
    ImGui::SliderFloat("Rotate degree axis-y", &m_degree_y, 0.f, 360.f);
    ImGui::SliderFloat("Rotate degree axis-z", &m_degree_z, 0.f, 360.f);
    ImGui::End();
  }

 private:
  float m_degree_x = 0.f;
  float m_degree_y = 0.f;
  float m_degree_z = 0.f;
};

class Simple3DLayer : public Layer {
 public:
  Simple3DLayer(ControlLayer* control) : Layer("Simple3DLayer"), m_control(control) {}
  ~Simple3DLayer() override = default;

 protected:
  void OnAttach() override {
    m_camera = Camera::MakePerspectiveCamera(glm::radians(60.f), 4.f / 3.f, 0.001f, 1000.f);
    m_camera->SetPosition({0.f, 0.f, 10.f});

    m_global_buffer = GetGraphicsContext()->CreateUniformBuffer(sizeof(glm::mat4));

    m_box_geometry = Geometry::MakeBox();

    m_box_geometry->InitBuffer(GetGraphicsContext());

    m_color_pipeline = example::ColorMaterial::CreatePipeline(GetGraphicsContext());
    m_color_material = std::make_unique<example::ColorMaterial>(glm::vec4(1.f, 0.f, 0.f, 1.f), m_color_pipeline.get());

    m_color_material->Init(GetGraphicsContext());

    m_box_mesh = std::make_unique<SimpleMesh>(m_box_geometry.get(), m_color_material.get());

    m_box_mesh->Init(GetGraphicsContext());
  }

  void OnUpdate(float tm) override {
    auto pv = m_camera->GetCameraMatrix();
    m_global_buffer->UploadData(&pv, sizeof(glm::mat4), 0);

    auto cmd = GetGraphicsContext()->CurrentCommandBuffer();

    glm::vec3 rotate{
        glm::radians(m_control->CurrentDegreeX()),
        glm::radians(m_control->CurrentDegreeY()),
        glm::radians(m_control->CurrentDegreeZ()),
    };

    m_box_mesh->SetRotation(rotate);

    m_box_mesh->Bind(cmd);

    std::vector<gpu::DescriptorBinding> bindings;
    bindings.emplace_back(gpu::DescriptorBinding{0, m_global_buffer.get()});

    // global sets
    m_box_mesh->UpdateDescriptorSet(0, bindings);

    // push constants
    BoxPushConstBlock block;
    block.eye = m_camera->GetPosition();
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

  void OnEvent(const Event* event) override {
    if (event->GetType() == EventType::KeyPressed) {
      auto key_event = dynamic_cast<const KeyPressEvent*>(event);

      auto code = key_event->GetKeyCode();

      if (code == KeyCode::W) {
        auto forward = m_camera->GetForward();
        auto pos = m_camera->GetPosition();

        pos += forward * 0.1f;

        m_camera->SetPosition(pos);
      } else if (code == KeyCode::S) {
        auto forward = m_camera->GetForward();
        auto pos = m_camera->GetPosition();

        pos -= forward * 0.1f;

        m_camera->SetPosition(pos);
      } else if (code == KeyCode::A) {
        auto rotate = m_camera->GetRotation();

        rotate.y -= glm::radians(5.f);

        m_camera->SetRotation(rotate);
      } else if (code == KeyCode::D) {
        auto rotate = m_camera->GetRotation();

        rotate.y += glm::radians(5.f);

        m_camera->SetRotation(rotate);
      }
    }
  }

 private:
  ControlLayer* m_control;
  std::shared_ptr<Camera> m_camera;
  std::unique_ptr<Geometry> m_box_geometry;
  std::unique_ptr<gpu::Pipeline> m_color_pipeline;
  std::unique_ptr<Material> m_color_material;
  std::unique_ptr<Mesh> m_box_mesh;
  std::unique_ptr<gpu::UniformBuffer> m_global_buffer;
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  HEX_INFO("create app instance 0x{:x}", reinterpret_cast<uintptr_t>(app));

  app->GetWindow()->SetClearColor(glm::vec4{0.f, 0.f, 0.f, 0.f});

  auto control = std::make_shared<ControlLayer>();

  app->PushLayer(std::make_shared<Simple3DLayer>(control.get()));

  app->PushLayer(control);

  app->Run();

  return 0;
}
