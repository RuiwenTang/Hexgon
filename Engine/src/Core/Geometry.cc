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

#include <Hexgon/Core/Geometry.hpp>
#include <Hexgon/Core/GraphicsContext.hpp>

namespace hexgon {

void Geometry::Build() {
  m_vertex.clear();
  m_index.clear();

  OnBuild();
}

void Geometry::InitBuffer(GraphicsContext *ctx) {
  if (m_gpu_vertex && m_gpu_index) {
    return;
  }

  m_gpu_vertex = ctx->CreateVertexBuffer(GetBufferLayout());

  m_gpu_vertex->Resize(GetVertexDataSize());

  m_gpu_vertex->UploadData(m_vertex.data(), GetIndexDataSize(), 0);

  m_gpu_index = ctx->CreateIndexBuffer();

  m_gpu_index->Resize(GetIndexDataSize());

  m_gpu_index->UploadData(m_index.data(), GetIndexDataSize(), 0);
}

void Geometry::BindCMD(gpu::CommandBuffer *cmd) {
  cmd->BindVertexBuffer(m_gpu_vertex.get(), 0);

  cmd->BindIndexBuffer(m_gpu_index.get());
}

}  // namespace hexgon
