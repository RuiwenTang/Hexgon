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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_LAYER_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_LAYER_HPP_

#include <Hexgon/Core/Event.hpp>
#include <Hexgon/Macro.hpp>
#include <string>

namespace hexgon {

class GraphicsContext;
class Application;

class HEX_API Layer {
  friend class Application;

 public:
  Layer(std::string name) : m_name(std::move(name)) {}

  virtual ~Layer() = default;

  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnUpdate(float tm) = 0;
  virtual void OnEvent(const Event* event) = 0;

  std::string const& GetLayerName() const { return m_name; }

  GraphicsContext* GetGraphicsContext() const { return m_context; }

 protected:
  Application* GetApplication() const { return m_application; }

 private:
  std::string m_name;
  GraphicsContext* m_context = nullptr;
  Application* m_application = nullptr;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_LAYER_HPP_