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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_GRAPHICS_CONTEXT_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_GRAPHICS_CONTEXT_HPP_

#include <Hexgon/GPU/Buffer.hpp>
#include <Hexgon/GPU/CommandBuffer.hpp>
#include <Hexgon/GPU/Formats.hpp>
#include <Hexgon/GPU/Pipeline.hpp>
#include <Hexgon/GPU/Texture.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace hexgon {

class GraphicsContext {
 public:
  enum class API { OpenGL, Vulkan };

  virtual ~GraphicsContext() = default;

  virtual void Init() = 0;
  virtual void Destroy() = 0;
  virtual void BeginFrame(glm::vec4 const& clear_color) = 0;
  virtual void SwapBuffers() = 0;

  virtual gpu::SampleCount GetSampleCount() = 0;

  virtual std::unique_ptr<gpu::Pipeline> CreatePipeline(gpu::PipelineInfo const& info) = 0;

  virtual std::unique_ptr<gpu::VertexBuffer> CreateVertexBuffer(gpu::BufferLayout const& layout) = 0;

  virtual std::unique_ptr<gpu::IndexBuffer> CreateIndexBuffer() = 0;

  virtual std::unique_ptr<gpu::UniformBuffer> CreateUniformBuffer(size_t size) = 0;

  virtual std::unique_ptr<gpu::Texture> CreateTexture(gpu::PixelFormat format, gpu::TextureUsage usage,
                                                      gpu::TextureType type) = 0;

  virtual std::vector<gpu::ColorAttachmentDescriptor> ScreenColorAttachment() = 0;

  virtual std::vector<gpu::DepthAttachmentDescriptor> ScreenDepthAttachment() = 0;

  virtual gpu::RenderPass* ScreenRenderPass() = 0;

  virtual gpu::CommandBuffer* CurrentCommandBuffer() = 0;

  static std::unique_ptr<GraphicsContext> Create(void* window, API api);
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_GRAPHICS_CONTEXT_HPP_
