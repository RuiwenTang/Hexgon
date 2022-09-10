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

#ifndef ENGINE_INCLUDE_HEXGON_OBJECT_MESH_HPP_
#define ENGINE_INCLUDE_HEXGON_OBJECT_MESH_HPP_

#include <Hexgon/Macro.hpp>
#include <Hexgon/Object/Object3D.hpp>
#include <vector>

namespace hexgon {

namespace gpu {

class CommandBuffer;
class Pipeline;

struct DescriptorBinding;

}  // namespace gpu

class Geometry;
class Material;
class GraphicsContext;

class HEX_API Mesh : public Object3D {
 public:
  Mesh(Geometry* geometry, Material* material) : Object3D(), m_geometry(geometry), m_material(material) {}

  ~Mesh() override;

  void AddChild(Mesh* child);

  void RemoveChild(Mesh* child);

  void Init(GraphicsContext* ctx);

  void Bind(gpu::CommandBuffer* cmd);

  void UpdateDescriptorSet(uint32_t slot, std::vector<gpu::DescriptorBinding> const& bindings);

  void UpdatePushConstant(uint32_t offset, uint32_t size, void* data);

  void Draw(gpu::CommandBuffer* cmd, glm::mat4 const& transform = glm::identity<glm::mat4>());

  Mesh* GetParent() const { return m_parent; }

  Geometry* GetGeometry() const { return m_geometry; }

  Material* GetMaterial() const { return m_material; }

 protected:
  void OnSetPosition(const glm::vec3& pos) override {}

  void OnSetRotation(const glm::vec3& rotation) override {}

  void OnSetScale(const glm::vec3& scale) override {}

  gpu::Pipeline* GetPipeline() const;

  virtual void OnInit(GraphicsContext* ctx) = 0;

  virtual void OnPrepareDraw() = 0;

 private:
  Geometry* m_geometry;
  Material* m_material;
  Mesh* m_parent = nullptr;
  std::vector<Mesh*> m_children = {};
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_OBJECT_MESH_HPP_