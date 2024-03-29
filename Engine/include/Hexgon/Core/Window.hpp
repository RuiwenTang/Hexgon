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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_WINDOW_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_WINDOW_HPP_

#include <Hexgon/Macro.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace hexgon {

class KeyEvent;
class MouseEvent;
class CharEvent;

class HEX_API WindowDelegate {
 public:
  virtual ~WindowDelegate() = default;

  virtual void OnWindowResize(int32_t width, int32_t height) = 0;
  virtual void OnWindowClose() = 0;
  virtual void OnWindowUpdate() = 0;
  virtual void OnKeyEvent(KeyEvent* event) = 0;
  virtual void OnMouseEvent(MouseEvent* event) = 0;
  virtual void OnCharEvent(CharEvent* event) = 0;
};

class HEX_API Window {
 public:
  Window(std::string title, uint32_t width, uint32_t height)
      : m_title(std::move(title)), m_width(width), m_height(height) {}

  virtual ~Window() = default;

  uint32_t GetWidth() const { return m_width; }
  uint32_t GetHeight() const { return m_height; }
  glm::vec2 GetDisplayScale() const { return m_display_scale; }

  std::string const& GetTitle() const { return m_title; }

  static std::unique_ptr<Window> Create(std::string title, uint32_t width, uint32_t height);

  virtual void SetVSync(bool enabled) = 0;

  virtual void* GetNativeWindow() const = 0;

  virtual void Show() = 0;

  virtual void Shutdown() = 0;

  void SetDelegate(WindowDelegate* delegate) { m_delegate = delegate; }

  void SetClearColor(glm::vec4 const& color) { m_clear_color = color; }

  glm::vec4 const& GetClearColor() const { return m_clear_color; }

 protected:
  void SetDisplayScale(glm::vec2 const& scale) { m_display_scale = scale; }

 private:
  std::string m_title;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
  glm::vec2 m_display_scale = {1.f, 1.f};
  glm::vec4 m_clear_color = {0.f, 0.f, 0.f, 0.f};

 protected:
  WindowDelegate* m_delegate = nullptr;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_WINDOW_HPP_
