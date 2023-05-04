#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/event/event.hpp>

namespace Hexgon {
class Layer {
 public:
  Layer(const std::string& name = "Layer") : m_name(name) {}
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(float ts) {}
  virtual void OnEvent(Event& event) {}

  const std::string& GetName() const { return m_name; }

 protected:
  std::string m_name;
};
}  // namespace Hexgon