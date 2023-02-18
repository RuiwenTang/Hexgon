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

#ifndef ENGINE_INCLUDE_HEXGON_GUI_IMGUI_LAYER_HPP_
#define ENGINE_INCLUDE_HEXGON_GUI_IMGUI_LAYER_HPP_

#include <Hexgon/Core/Layer.hpp>
#include <memory>

namespace hexgon {

namespace gpu {

class Texture;
class Pipeline;
class VertexBuffer;
class IndexBuffer;

}  // namespace gpu

class HEX_API ImguiLayer : public Layer {
 public:
  ImguiLayer();
  ~ImguiLayer() override = default;

 protected:
  void OnAttach() override;

  void OnUpdate(float tm) override;

  void OnDetach() override;

  void OnEvent(const Event *event) override;

  virtual void OnImguiInit() = 0;

  virtual void OnDrawImgui(float tm) = 0;

 private:
  void InitImGui();

  void InitImGuiPipeline();

  void InitImguiBuffers();

  void FlushImgui();

 private:
  bool m_imgui_initialized = false;
  std::unique_ptr<gpu::Texture> m_font_texture;
  std::unique_ptr<gpu::Pipeline> m_imgui_pipeline;
  std::unique_ptr<gpu::VertexBuffer> m_vertex_buffer;
  std::unique_ptr<gpu::IndexBuffer> m_index_buffer;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_GUI_IMGUI_LAYER_HPP_
