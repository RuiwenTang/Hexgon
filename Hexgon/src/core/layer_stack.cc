#include <hexgon/core/layer_stack.hpp>

namespace Hexgon {

void LayerStack::PushLayer(Ref<Layer> layer) {
  m_layers.emplace_back(std::move(layer));
}

void LayerStack::PopLayer(Ref<Layer> layer) {
  auto it = std::find(m_layers.begin(), m_layers.end(), layer);

  if (it != m_layers.end()) {
    layer->OnDetach();
    m_layers.erase(it);
  }
}

void LayerStack::ForEachLayer(std::function<void(Layer*)>&& f) {
  for (const auto& l : m_layers) {
    f(l.get());
  }
}

}  // namespace Hexgon