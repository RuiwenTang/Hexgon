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

#include <imgui.h>

#include <Hexgon/Hexgon.hpp>

using namespace hexgon;

class ImguiLayer : public Layer {
 public:
  ImguiLayer() : Layer("ImguiLayer") {}
  ~ImguiLayer() override = default;

 protected:
  void OnAttach() override {
    HEX_INFO("ImguiLayer OnAttach");
    InitImGui();
  }

  void OnUpdate(float tm) override {
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize.x = GetApplication()->GetWindow()->GetWidth();
    io.DisplaySize.y = GetApplication()->GetWindow()->GetHeight();

    ImGui::NewFrame();
    DrawImGui();
    ImGui::Render();
    FlushImGui();
  }

  void OnDetach() override {
    HEX_INFO("ImguiLayer OnDetach");
    ImGui::DestroyContext();

    m_font_texture.reset();
  }

  void OnEvent(const Event* event) override {}

 private:
  void InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    // Fonts
    // io.Fonts->AddFontFromFileTTF(RESOURCES_DIR "/Fonts/DroidSans.ttf", 16.f);

    uint8_t* pixels;
    int32_t width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    m_font_texture = GetGraphicsContext()->CreateTexture(
        gpu::PixelFormat::R8G8B8A8UNormInt, gpu::TextureUsage::ShaderRead, gpu::TextureType::kImageTexture);
  
    m_font_texture->Resize(width, height);

    m_font_texture->UploadData(0, 0, width, height, pixels);

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(m_font_texture.get()));
  }

  void DrawImGui() {
    ImGui::Begin("Hello, World!");

    bool checked = false;
    ImGui::Checkbox("Check box 1", &checked);

    ImGui::End();
  }

  void FlushImGui() {
    ImDrawData* draw_data = ImGui::GetDrawData();

    if (draw_data->TotalVtxCount > 0) {
      size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
      size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

      HEX_INFO("ImGui draw with vertex_count: {} and index_count: {} ", draw_data->TotalVtxCount,
               draw_data->TotalIdxCount);
      ;
    }
  }

 private:
  std::unique_ptr<gpu::Texture> m_font_texture;
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  app->PushLayer(std::make_shared<ImguiLayer>());

  app->Run();

  return 0;
}