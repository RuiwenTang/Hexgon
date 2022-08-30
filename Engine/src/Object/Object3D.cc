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

#include <Hexgon/Object/Object3D.hpp>

namespace hexgon {

void Object3D::SetPosition(const glm::vec3 &pos) {
  OnSetPosition(pos);

  m_pos = pos;
}

void Object3D::SetRotation(const glm::vec3 &rotation) {
  OnSetRotation(rotation);

  m_rotate = rotation;
}

void Object3D::SetScale(const glm::vec3 &scale) {
  OnSetScale(scale);

  m_scale = scale;
}

glm::mat4 Object3D::CalculateMatrix() const {
  glm::mat4 ret = glm::identity<glm::mat4>();
  ret = glm::rotate(ret, m_rotate.x, {1.f, 0.f, 0.f});
  ret = glm::rotate(ret, m_rotate.y, {0.f, 1.f, 0.f});
  ret = glm::rotate(ret, m_rotate.z, {0.f, 0.f, 1.f});

  ret = glm::translate(ret, m_pos);

  ret = glm::scale(ret, m_scale);

  return ret;
}

}  // namespace hexgon
