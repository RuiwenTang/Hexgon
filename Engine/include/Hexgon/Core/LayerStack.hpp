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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_LAYER_STACK_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_LAYER_STACK_HPP_

#include <Hexgon/Core/Layer.hpp>
#include <Hexgon/Macro.hpp>
#include <memory>
#include <vector>

namespace hexgon {

class HEX_API LayerStack final {
 public:
  using iterator = std::vector<std::shared_ptr<Layer>>::iterator;
  using const_iterator = std::vector<std::shared_ptr<Layer>>::const_iterator;

  LayerStack() = default;
  ~LayerStack();

  void PushLayer(std::shared_ptr<Layer> layer);

  void PopLayer(std::shared_ptr<Layer> const& layer);

  iterator begin() { return m_layers.begin(); }
  iterator end() { return m_layers.end(); }

  const_iterator begin() const { return m_layers.begin(); }
  const_iterator end() const { return m_layers.end(); }

 private:
  std::vector<std::shared_ptr<Layer>> m_layers = {};
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_LAYER_STACK_HPP_