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

#include <Hexgon/Object/Camera.hpp>

namespace hexgon {

Camera::Camera(glm::mat4 const& project)
    : Object3D(), m_proj(project), m_target(), m_up(0.f, -1.f, 0.f), m_forward(0.f, 0.f, -1.f) {
  SetPosition({0.f, 0.f, 1.f});
}

glm::mat4 Camera::GetCameraMatrix() {
  auto matrix = CalculateMatrix();

  m_up = matrix * glm::vec4(m_up, 0.f);
  glm::vec3 forward = matrix * glm::vec4(m_forward, 0.f);

  m_target = GetPosition() + GetForward();

  glm::mat4 view = glm::lookAt(GetPosition(), m_target, m_up);

  return m_proj * view;
}

glm::vec3 Camera::GetForward() const {
  glm::vec3 forward = CalculateMatrix() * glm::vec4(m_forward, 0.f);

  return glm::normalize(forward);
}

void Camera::OnSetPosition(const glm::vec3& pos) {}

void Camera::OnSetRotation(const glm::vec3& rotation) {}

void Camera::OnSetScale(const glm::vec3& scale) {}

std::shared_ptr<Camera> Camera::MakePerspectiveCamera(float fov, float aspect, float near, float far) {
  glm::mat4 project = glm::perspective(fov, aspect, near, far);

  return std::make_shared<Camera>(project);
}

}  // namespace hexgon
