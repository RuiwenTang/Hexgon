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

#pragma once

#include <Hexgon/Hexgon.hpp>
#include <mutex>

#include "Layer/GUILayer.hpp"

class RenderLayer : public hexgon::Layer, public GUILayer::Callback {
 public:
  RenderLayer() : Layer("SimpleTracer") {}
  ~RenderLayer() override = default;

  void OnAttach() override;

  void OnDetach() override;

  void OnUpdate(float tm) override;

  void OnEvent(const hexgon::Event* event) override {}

  void OnRender() override;

 private:
  void InitPipeline();
  void InitBuffers();
  void InitImage();
  void UploadTexture();

  void UploadTextureIfNeed();

  void DoRender();

 private:
  std::unique_ptr<hexgon::gpu::Pipeline> m_pipeline;
  std::unique_ptr<hexgon::gpu::VertexBuffer> m_vertex_buffer;
  std::unique_ptr<hexgon::gpu::IndexBuffer> m_index_buffer;
  std::unique_ptr<hexgon::gpu::Texture> m_gpu_texture;
  std::shared_ptr<hexgon::io::Image> m_texture;
  bool m_rendering = false;
  bool m_image_dirty = false;
  std::mutex m_mutex = {};
};
