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

#ifndef ENGINE_INCLUDE_HEXGON_OBJECT_OBJECT3D_HPP
#define ENGINE_INCLUDE_HEXGON_OBJECT_OBJECT3D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace hexgon {

class Object3D {
 public:
  Object3D() = default;
  virtual ~Object3D() = default;

  void SetPosition(glm::vec3 const& pos);
  void SetRotation(glm::vec3 const& rotation);
  void SetScale(glm::vec3 const& scale);

  glm::vec3 const& GetPosition() const { return m_pos; }
  glm::vec3 const& GetRotation() const { return m_rotate; }
  glm::vec3 const& GetScale() const { return m_scale; }

  glm::mat4 CalculateMatrix() const;

 protected:
  virtual void OnSetPosition(glm::vec3 const& pos) = 0;
  virtual void OnSetRotation(glm::vec3 const& rotation) = 0;
  virtual void OnSetScale(glm::vec3 const& scale) = 0;

 private:
  glm::vec3 m_pos = {};
  glm::vec3 m_rotate = {};
  glm::vec3 m_scale = {1.f, 1.f, 1.f};
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_OBJECT_OBJECT3D_HPP
