#include "example_layer.hpp"

ExampleLayer::ExampleLayer() : Hexgon::Layer("ExampleLayer") {}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::OnAttach() {
  HEX_INFO("ExampleLayer OnAttach");

  InitShader();
}

void ExampleLayer::OnDetach() { HEX_INFO("ExampleLlayer OnDetach"); }

void ExampleLayer::InitShader() {
  Hexgon::ShaderDescriptor desc{};
  desc.entry_point = "vMain";
  desc.type = Hexgon::ShaderType::kVertex;
  desc.sourceType = Hexgon::ShaderSourceType::kCodeFile;
  desc.source = "assets/triangle.metal";

  m_shader = Hexgon::Application::Get().GetRenderSystem()->CreateShader(desc);

  if (m_shader) {
    HEX_INFO("Shader init success");
  } else {
    HEX_INFO("Shader init failed");
  }
}