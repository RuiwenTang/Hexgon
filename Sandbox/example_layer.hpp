#include <hexgon/hexgon.hpp>

class ExampleLayer : public Hexgon::Layer {
 public:
  ExampleLayer();
  ~ExampleLayer() override;

  void OnAttach() override;

  void OnDetach() override;
};