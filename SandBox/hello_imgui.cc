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

#include "ShaderSource.hpp"

using namespace hexgon;

struct PushConstBlock {
  glm::vec2 scale;
  glm::vec2 translate;
};

class ImguiLayer : public Layer {
 public:
  ImguiLayer() : Layer("ImguiLayer") {}
  ~ImguiLayer() override = default;

 protected:
  void OnAttach() override {
    HEX_INFO("ImguiLayer OnAttach");
    InitImGui();
    InitImguiPipeline();
    InitImGuiBuffers();
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
    m_vertex_buffer.reset();
    m_index_buffer.reset();
    m_imgui_pipeline.reset();
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
    ImGuiIO& io = ImGui::GetIO();

    ImDrawData* draw_data = ImGui::GetDrawData();

    if (draw_data->TotalVtxCount <= 0) {
      return;
    }

    size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
    size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

    m_vertex_buffer->Resize(vertex_size);
    m_index_buffer->Resize(index_size);

    size_t vertex_offset = 0;
    size_t index_offset = 0;

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
      auto cmd_list = draw_data->CmdLists[n];

      m_vertex_buffer->UploadData(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                                  vertex_offset);
      m_index_buffer->UploadData(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), index_offset);

      vertex_offset += cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
      index_offset += cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
    }

    auto cmd = GetGraphicsContext()->CurrentCommandBuffer();

    cmd->BindPipeline(m_imgui_pipeline.get());

    cmd->BindVertexBuffer(m_vertex_buffer.get(), 0);

    cmd->BindIndexBuffer(m_index_buffer.get());

    PushConstBlock push_const;
    push_const.scale = glm::vec2(2.f / io.DisplaySize.x, 2.f / io.DisplaySize.y);
    push_const.translate = glm::vec2(-1.f);

    m_imgui_pipeline->UpdatePushConstant(0, sizeof(PushConstBlock), &push_const);

    std::vector<gpu::DescriptorBinding> bindings;
    bindings.emplace_back(gpu::DescriptorBinding{m_font_texture.get()});
    m_imgui_pipeline->UpdateDescriptorSet(0, bindings);

    vertex_offset = 0;
    index_offset = 0;

    for (int32_t n = 0; n < draw_data->CmdListsCount; n++) {
      auto cmd_list = draw_data->CmdLists[n];
      for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++) {
        auto p_cmd = &cmd_list->CmdBuffer[j];

        cmd->DrawIndexed(p_cmd->ElemCount, 1, index_offset, 0);
        index_offset += p_cmd->ElemCount;
      }
    }
  }

  void InitImguiPipeline() {
    gpu::PipelineInfo info;
    // sample count
    info.sample_count = GetGraphicsContext()->GetSampleCount();
    // vertex shader
    info.shaders.emplace_back(
        gpu::Shader(gpu::Shader::Stage::Vertex, (const char*)imgui_vert_spv, imgui_vert_spv_size));
    // fragment shader
    info.shaders.emplace_back(
        gpu::Shader(gpu::Shader::Stage::Fragment, (const char*)imgui_frag_spv, imgui_frag_spv_size));

    // vertex stride
    info.vertex_binding.emplace_back(gpu::VertexBinding{0, sizeof(ImDrawVert)});
    // pos
    info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 0, gpu::DataType::Float2, offsetof(ImDrawVert, pos)});
    // uv
    info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 1, gpu::DataType::Float2, offsetof(ImDrawVert, uv)});
    // color
    info.attr_desc.emplace_back(gpu::VertexAttributeDescriptor{0, 2, gpu::DataType::Char4, offsetof(ImDrawVert, col)});

    // attachments
    info.color_attachment = GetGraphicsContext()->ScreenColorAttachment();
    info.depth_attachment = GetGraphicsContext()->ScreenDepthAttachment();

    // render pass
    info.render_pass = GetGraphicsContext()->ScreenRenderPass();

    m_imgui_pipeline = GetGraphicsContext()->CreatePipeline(info);
  }

  void InitImGuiBuffers() {
    gpu::BufferLayout layout({
        gpu::BufferElement("pos", gpu::DataType::Float2, offsetof(ImDrawVert, pos)),
        gpu::BufferElement("uv", gpu::DataType::Float2, offsetof(ImDrawVert, uv)),
        gpu::BufferElement("col", gpu::DataType::Char4, offsetof(ImDrawVert, col)),
    });

    m_vertex_buffer = GetGraphicsContext()->CreateVertexBuffer(layout);

    m_index_buffer = GetGraphicsContext()->CreateIndexBuffer();
  }

 private:
  std::unique_ptr<gpu::Texture> m_font_texture;
  std::unique_ptr<gpu::Pipeline> m_imgui_pipeline;
  std::unique_ptr<gpu::VertexBuffer> m_vertex_buffer;
  std::unique_ptr<gpu::IndexBuffer> m_index_buffer;
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  app->GetWindow()->SetClearColor(glm::vec4{1.f, 1.f, 1.f, 1.f});

  app->PushLayer(std::make_shared<ImguiLayer>());

  app->Run();

  return 0;
}