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

#include <Hexgon/Core/Event.hpp>
#include <Hexgon/Core/Window.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "LogPrivate.hpp"

namespace hexgon {

namespace {

static void GLFWErrorCallback(int32_t err, const char* desc) { HEX_CORE_ERROR("GLFW error ({0}): {1}", err, desc); }

}  // namespace

class GLFWWindowImpl : public Window {
 public:
  GLFWWindowImpl(std::string title, uint32_t width, uint32_t height)
      : Window(title, width, height), m_glfw_window(nullptr) {}
  ~GLFWWindowImpl() override {
    if (m_glfw_window) {
      glfwDestroyWindow(m_glfw_window);
    }
  }

  void SetVSync(bool enabled) override {
    if (enabled) {
      glfwSwapInterval(1);
    } else {
      glfwSwapInterval(0);
    }
  }

  void Show() override {
    while (m_running && !glfwWindowShouldClose(m_glfw_window)) {
      glfwPollEvents();

      m_context->BeginFrame(GetClearColor());

      for (auto client : m_clients) {
        client->OnWindowUpdate();
      }

      m_context->SwapBuffers();
    }

    for (auto client : m_clients) {
      client->OnWindowClose();
    }

    m_context->Destroy();
  }

  void Shutdown() override { m_running = false; }

  void Init() {
    glfwInit();

    // error callback
    glfwSetErrorCallback(GLFWErrorCallback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_glfw_window = glfwCreateWindow(GetWidth(), GetHeight(), GetTitle().c_str(), nullptr, nullptr);

    int32_t display_w;
    int32_t display_h;
    glfwGetFramebufferSize(m_glfw_window, &display_w, &display_h);

    glm::vec2 scale{
        static_cast<float>(display_w) / GetWidth(),
        static_cast<float>(display_h) / GetHeight(),
    };

    SetDisplayScale(scale);

    glfwSetWindowUserPointer(m_glfw_window, this);

    glfwSetKeyCallback(m_glfw_window, WindowKeyCallback);
    glfwSetCursorPosCallback(m_glfw_window, WindowCursorCallback);
    glfwSetMouseButtonCallback(m_glfw_window, WindowMouseButtonCallback);
    glfwSetCharCallback(m_glfw_window, WindowCharCallback);
  }

  void* GetNativeWindow() const override { return m_glfw_window; }

  static void WindowKeyCallback(GLFWwindow* window, int32_t key, int32_t scan_code, int32_t action, int mods) {
    auto platform_window = reinterpret_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));

    std::unique_ptr<KeyEvent> event;
    switch (action) {
      case GLFW_PRESS:
      case GLFW_REPEAT:
        event = std::make_unique<KeyPressEvent>(KeyCode::Code(key));
        break;
      case GLFW_RELEASE:
        event = std::make_unique<KeyReleaseEvent>(KeyCode::Code(key));
        break;
    }

    if (!event) {
      return;
    }

    for (auto client : platform_window->m_clients) {
      client->OnKeyEvent(event.get());
    }
  }

  static void WindowCursorCallback(GLFWwindow* window, double offset_x, double offset_y) {
    auto platform_window = reinterpret_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));
    platform_window->m_cursor_x = offset_x;
    platform_window->m_cursor_y = offset_y;

    MouseMovedEvent event(static_cast<float>(offset_x), static_cast<float>(offset_y));

    for (auto client : platform_window->m_clients) {
      client->OnMouseEvent(&event);
    }
  }

  static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto platform_window = reinterpret_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));

    float x = static_cast<float>(platform_window->m_cursor_x);
    float y = static_cast<float>(platform_window->m_cursor_y);

    std::unique_ptr<MouseEvent> event;

    if (action == GLFW_PRESS) {
      event = std::make_unique<MousePressedEvent>(x, y, static_cast<MouseCode>(button));
    } else {
      event = std::make_unique<MouseReleasedEvent>(x, y, static_cast<MouseCode>(button));
    }

    for (auto client : platform_window->m_clients) {
      client->OnMouseEvent(event.get());
    }
  }

  static void WindowCharCallback(GLFWwindow* window, unsigned int c) {
    auto platform_window = reinterpret_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));

    auto event = std::make_unique<CharEvent>(c);

    for (auto client : platform_window->m_clients) {
      client->OnCharEvent(event.get());
    }
  }

 private:
  GLFWwindow* m_glfw_window;
  bool m_running = true;
  double m_cursor_x = 0.0;
  double m_cursor_y = 0.0;
};

std::unique_ptr<Window> Window::Create(std::string title, uint32_t width, uint32_t height) {
  auto window = std::make_unique<GLFWWindowImpl>(std::move(title), width, height);

  window->Init();

  window->m_context = GraphicsContext::Create(window->GetNativeWindow(), GraphicsContext::API::Vulkan);
  window->m_context->Init();

  return window;
}

}  // namespace hexgon
