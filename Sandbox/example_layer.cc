#include "example_layer.hpp"

ExampleLayer::ExampleLayer() : Hexgon::Layer("ExampleLayer") {}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::OnAttach() {
  HEX_INFO("ExampleLayer OnAttach");

  InitShader();
}

void ExampleLayer::OnDetach() { HEX_INFO("ExampleLlayer OnDetach"); }

void ExampleLayer::InitShader() {
  Hexgon::ShaderDescriptor vs_desc{};
  vs_desc.entry_point = "vMain";
  vs_desc.type = Hexgon::ShaderType::kVertex;
  vs_desc.sourceType = Hexgon::ShaderSourceType::kCodeFile;
  vs_desc.source = "assets/triangle.metal";

  vs_desc.vertex.inputAttribs.emplace_back(Hexgon::VertexAttribute{
      "position",
      Hexgon::Format::RG32Float,
      0,
      0,
      0,
      2 * sizeof(float),
      0,
  });

  Hexgon::ShaderDescriptor fs_desc{};
  fs_desc.entry_point = "fMain";
  fs_desc.type = Hexgon::ShaderType::kFragment;
  fs_desc.sourceType = Hexgon::ShaderSourceType::kCodeFile;
  fs_desc.source = "assets/triangle.metal";

  auto vs = Hexgon::Application::Get().GetRenderSystem()->CreateShader(vs_desc);
  auto fs = Hexgon::Application::Get().GetRenderSystem()->CreateShader(fs_desc);

  if (vs && fs) {
    HEX_INFO("Shader init success");
  } else {
    HEX_INFO("Shader init failed");
  }
}