#pragma once

#include <functional>
#include <hexgon/core/base.hpp>
#include <hexgon/core/layer.hpp>
#include <vector>

namespace Hexgon {

class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack() = default;

  void PushLayer(Ref<Layer> layer);
  void PopLayer(Ref<Layer> layer);

  void ForEachLayer(std::function<void(Layer*)>&& f);

 private:
  std::vector<Ref<Layer>> m_layers = {};
};

}  // namespace Hexgon
