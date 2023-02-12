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
#include <Hexgon/Core/Material.hpp>
#include <Hexgon/Object/Mesh.hpp>
#include <algorithm>

namespace hexgon {

Mesh::~Mesh() = default;

void Mesh::AddChild(Mesh* child) {
  auto it = std::find_if(m_children.begin(), m_children.end(), [child](Mesh* m) { return m == child; });

  if (it != m_children.end()) {
    return;
  }

  m_children.emplace_back(child);

  child->m_parent = this;
}

void Mesh::RemoveChild(Mesh* child) {
  auto it = std::find_if(m_children.begin(), m_children.end(), [child](Mesh* m) { return m == child; });

  if (it == m_children.end()) {
    return;
  }

  (*it)->m_parent = nullptr;

  m_children.erase(it);
}

void Mesh::Init(GraphicsContext* ctx) {
  OnInit(ctx);

  for (auto const& child : m_children) {
    child->Init(ctx);
  }
}

void Mesh::Bind(gpu::CommandBuffer* cmd) {
  m_geometry->BindCMD(cmd);
  m_material->BindCMD(cmd);
}

void Mesh::UpdateDescriptorSet(uint32_t slot, const std::vector<gpu::DescriptorBinding>& bindings) {
  m_material->GetPipeline()->UpdateDescriptorSet(slot, bindings);
}

void Mesh::UpdatePushConstant(uint32_t offset, uint32_t size, void* data) {
  m_material->GetPipeline()->UpdatePushConstant(offset, size, data);
}

void Mesh::Draw(gpu::CommandBuffer* cmd, glm::mat4 const& transform) {
  OnPrepareDraw();

  cmd->DrawIndexed(m_geometry->GetIndexCount(), 1, 0, 0);
}

gpu::Pipeline* Mesh::GetPipeline() const { return m_material->GetPipeline(); }

}  // namespace hexgon
