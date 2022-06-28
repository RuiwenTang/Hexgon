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

#ifndef ENGINE_INCLUDE_HEXGON_WINDOW_HPP_
#define ENGINE_INCLUDE_HEXGON_WINDOW_HPP_

#include <Hexgon/GraphicsContext.hpp>
#include <Hexgon/Macro.hpp>
#include <memory>
#include <string>
#include <vector>

namespace hexgon {

class KeyEvent;

class HEX_API WindowClient {
 public:
  virtual ~WindowClient() = default;

  virtual void OnWindowResize(int32_t width, int32_t height) = 0;
  virtual void OnWindowClose() = 0;
  virtual void OnKeyEvent(KeyEvent* event) = 0;
};

class HEX_API Window {
 public:
  Window(std::string title, uint32_t width, uint32_t height)
      : m_title(std::move(title)), m_width(width), m_height(height) {}

  virtual ~Window() = default;

  uint32_t GetWidth() const { return m_width; }
  uint32_t GetHeight() const { return m_height; }

  std::string const& GetTitle() const { return m_title; }

  static std::unique_ptr<Window> Create(std::string title, uint32_t width, uint32_t height);

  virtual void SetVSync(bool enabled) = 0;

  virtual void* GetNativeWindow() const = 0;

  virtual void Show() = 0;

  virtual void Shutdown() = 0;

  void AddClient(WindowClient* client) { m_clients.emplace_back(client); }

 private:
  std::string m_title;
  uint32_t m_width = 0;
  uint32_t m_height = 0;

 protected:
  std::vector<WindowClient*> m_clients = {};
  std::unique_ptr<GraphicsContext> m_context = {};
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_WINDOW_HPP_
