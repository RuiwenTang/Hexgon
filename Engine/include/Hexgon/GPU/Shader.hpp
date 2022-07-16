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

#ifndef ENGINE_INCLUDE_HEXGON_GPU_SHADER_HPP_
#define ENGINE_INCLUDE_HEXGON_GPU_SHADER_HPP_

#include <string>

namespace hexgon {
namespace gpu {

class Shader final {
 public:
  enum class Stage {
    Vertex,
    Fragment,
    Geometry,
    Compute,
  };

  Shader(Stage stage, const char* source, size_t size) : m_stage(stage), m_source(source), m_size(size) {}

  ~Shader() = default;

  Stage GetShaderStage() const { return m_stage; }

  const char* GetSource() const { return m_source; }

  size_t GetSize() const { return m_size; }

 private:
  Stage m_stage;
  const char* m_source;
  size_t m_size;
};

}  // namespace gpu

}  // namespace hexgon

#endif  // ENGINE_ICNLUDE_HEXGON_GPU_SHADER_HPP_