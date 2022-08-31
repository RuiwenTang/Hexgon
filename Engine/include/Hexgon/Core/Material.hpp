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

#ifndef ENGINE_INCLUDE_HEXGON_CORE_MATERIAL_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_MATERIAL_HPP_

#include <Hexgon/GPU/Pipeline.hpp>
#include <Hexgon/Macro.hpp>

namespace hexgon {

namespace gpu {
class CommandBuffer;
}

class GraphicsContext;

class HEX_API Material {
 public:
  Material(gpu::PipelineInfo const& info) : m_pipeline_info(info) {}

  virtual ~Material() = default;

  void Init(GraphicsContext* ctx);

  void BindCMD(gpu::CommandBuffer* cmd);

  gpu::Pipeline* GetPipeline() const { return m_pipeline.get(); }

 protected:
  virtual void OnPipelineInit() = 0;

  virtual void OnBindCMD(gpu::CommandBuffer* cmd) = 0;

 private:
  gpu::PipelineInfo m_pipeline_info;
  std::unique_ptr<gpu::Pipeline> m_pipeline;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_MATERIAL_HPP_
