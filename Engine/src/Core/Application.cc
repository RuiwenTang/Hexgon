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

#include <Hexgon/Core/Application.hpp>
#include <Hexgon/Core/Event.hpp>

#include "LogPrivate.hpp"

namespace hexgon {

Application* Application::g_instance = nullptr;

Application* Application::Create(std::string title, uint32_t width, uint32_t height) {
  if (g_instance) {
    // TODO assert failed
    HEX_CORE_ERROR("Application already created!!");
    return g_instance;
  }

  g_instance = new Application;

  // init window
  g_instance->m_window = Window::Create(std::move(title), width, height);

  g_instance->m_window->AddClient(g_instance);

  return g_instance;
}

Application* Application::Get() { return g_instance; }

void Application::Run() { m_window->Show(); }

void Application::PushLayer(std::shared_ptr<Layer> const& layer) {
  layer->m_context = m_window->GetContext();
  layer->m_application = this;
  m_layer_stack.PushLayer(layer);
}

void Application::PopLayer(std::shared_ptr<Layer> const& layer) {
  m_layer_stack.PopLayer(layer);

  layer->m_application = nullptr;
  layer->m_context = nullptr;
}

void Application::OnWindowResize(int32_t width, int32_t height) {
  HEX_CORE_INFO("Window Resize to { {}, {} }", width, height);
}

void Application::OnWindowClose() {
  for (auto const& it : m_layer_stack) {
    it->OnDetach();
  }
}

void Application::OnWindowUpdate() {
  for (auto const& it : m_layer_stack) {
    it->OnUpdate(0.f);
  }
}

void Application::OnKeyEvent(KeyEvent* event) {
  HEX_CORE_INFO("On Key event : {}", event->GetName());

  for (auto const& it : m_layer_stack) {
    it->OnEvent(event);
  }
}

void Application::OnMouseEvent(MouseEvent* event) {
  for (auto const& it : m_layer_stack) {
    it->OnEvent(event);
  }
}

}  // namespace hexgon
