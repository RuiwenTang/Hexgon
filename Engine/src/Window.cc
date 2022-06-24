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

#include <Hexgon/Window.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace hexgon {

class GLFWWindowImpl : public Window {
 public:
  GLFWWindowImpl(std::string title, uint32_t width, uint32_t height)
      : Window(title, width, height), m_glfw_window(nullptr) {}
  ~GLFWWindowImpl() override = default;

  void SetVSync(bool enabled) override {
    if (enabled) {
      glfwSwapInterval(1);
    } else {
      glfwSwapInterval(0);
    }
  }

  void Init() {}

  void* GetNativeWindow() const override { return m_glfw_window; }

 private:
  GLFWwindow* m_glfw_window;
};

std::unique_ptr<Window> Window::Create(std::string title, uint32_t width, uint32_t height) {
  auto window = std::make_unique<GLFWWindowImpl>(std::move(title), width, height);

  window->Init();

  return window;
}

}  // namespace hexgon
