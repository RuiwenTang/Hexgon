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

#ifndef ENGINE_INCLUDE_HEXGON_OBJECT_CAMERA_HPP_
#define ENGINE_INCLUDE_HEXGON_OBJECT_CAMERA_HPP_

#include <Hexgon/Macro.hpp>
#include <Hexgon/Object/Object3D.hpp>
#include <memory>

namespace hexgon {

class HEX_API Camera : public Object3D {
 public:
  Camera(glm::mat4 const& project);

  ~Camera() override = default;

  void SetTarget(glm::vec3 const& target) { m_target = target; }

  void SetUp(glm::vec3 const& up) { m_up = up; }

  glm::mat4 GetCameraMatrix();

  glm::vec3 GetForward() const;

  static std::shared_ptr<Camera> MakePerspectiveCamera(float fov, float aspect, float near, float far);

 protected:
  void OnSetPosition(const glm::vec3& pos) override;
  void OnSetRotation(const glm::vec3& rotation) override;
  void OnSetScale(const glm::vec3& scale) override;

 private:
  glm::mat4 m_proj;
  glm::vec3 m_target;
  glm::vec3 m_up;
  glm::vec3 m_forward;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_OBJECT_CAMERA_HPP_
