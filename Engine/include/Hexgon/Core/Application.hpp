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

#ifndef INCLUDE_HEXGON_CORE_APPLICATION_HPP_
#define INCLUDE_HEXGON_CORE_APPLICATION_HPP_

#include <Hexgon/Core/LayerStack.hpp>
#include <Hexgon/Core/Window.hpp>
#include <Hexgon/Macro.hpp>
#include <Hexgon/Render/RenderSystem.hpp>
#include <Hexgon/Render/SwapChain.hpp>
#include <memory>

namespace hexgon {

class HEX_API Application final : public WindowDelegate {
 public:
  ~Application() = default;

  static Application* Create(std::string title, uint32_t width = 800, uint32_t height = 600);

  static Application* Get();

  Window* GetWindow() const { return m_window.get(); }

  void Run();

  void PushLayer(std::shared_ptr<Layer> const& layer);
  void PopLayer(std::shared_ptr<Layer> const& layer);

  void OnWindowResize(int32_t width, int32_t height) override;
  void OnWindowClose() override;
  void OnWindowUpdate() override;
  void OnKeyEvent(KeyEvent* event) override;
  void OnMouseEvent(MouseEvent* event) override;
  void OnCharEvent(CharEvent* event) override;

 private:
  Application() = default;
  static Application* g_instance;

 private:
  std::unique_ptr<Window> m_window = {};
  std::unique_ptr<RenderSystem> m_render_system = {};
  std::unique_ptr<SwapChain> m_swap_chain = {};
  LayerStack m_layer_stack = {};
};

}  // namespace hexgon

#endif  // INCLUDE_HEXGON_CORE_APPLICATION_HPP_