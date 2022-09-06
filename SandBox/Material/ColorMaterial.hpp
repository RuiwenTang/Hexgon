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

#ifndef SANDBOX_MATERIAL_COLOR_MATERIAL_HPP_
#define SANDBOX_MATERIAL_COLOR_MATERIAL_HPP_

#include <Hexgon/Hexgon.hpp>

namespace example {

class ColorMaterial : public hexgon::Material {
 public:
  ~ColorMaterial() override = default;

  static std::unique_ptr<ColorMaterial> Create(hexgon::GraphicsContext* ctx, glm::vec4 const& color);

 protected:
  void OnPipelineInit(hexgon::GraphicsContext* ctx) override;

  void OnBindCMD(hexgon::gpu::CommandBuffer* cmd) override;

  void OnPrepareForDraw(std::vector<hexgon::gpu::DescriptorBinding>& bindings) override;

 private:
  ColorMaterial(glm::vec4 const& color, hexgon::gpu::PipelineInfo const& info)
      : hexgon::Material(info), m_color(color) {}

 private:
  glm::vec4 m_color;
  std::unique_ptr<hexgon::gpu::UniformBuffer> m_color_buffer;
};

}  // namespace example

#endif  // SANDBOX_MATERIAL_COLOR_MATERIAL_HPP_