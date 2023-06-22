#include <Hexgon/Hexgon.hpp>

class SimpleLayer : public hexgon::Layer {
 public:
  SimpleLayer() : Layer("Hello Layer") {}
  ~SimpleLayer() override = default;

  void OnAttach() override { HEX_INFO("Layer: {} OnAttach", GetLayerName()); }

  void OnDetach() override { HEX_INFO("Layer: {} OnDetach", GetLayerName()); }

  void OnUpdate(float tm) override {}

  void OnEvent(const hexgon::Event* event) override {
    HEX_INFO("Layer: {} OnEvent: name: {}", GetLayerName(), event->GetName());
  }
};

int main(int argc, const char** argv) {
  auto app = hexgon::Application::Create("Hello World");

  app->PushLayer(std::make_shared<SimpleLayer>());

  app->Run();

  return 0;
}