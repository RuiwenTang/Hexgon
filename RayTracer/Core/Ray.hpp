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

#pragma once

#include <Hexgon/Hexgon.hpp>
#include <cstdint>

class HEX_API Ray {
 public:
  Ray() : m_orig(), m_dir() {}
  Ray(glm::vec3 const& origin, glm::vec3 const& direction) : m_orig(origin), m_dir(glm::normalize(direction)) {}
  ~Ray() = default;

  glm::vec3 const& Origin() const { return m_orig; }
  glm::vec3 const& Direction() const { return m_dir; }

  glm::vec3 At(float t) const;

 private:
  glm::vec3 m_orig;
  glm::vec3 m_dir;
};

class HEX_API RayCamera {
 public:
  RayCamera(glm::vec3 const& origin, glm::vec3 const& target, glm::vec3 const& up, float fov, uint32_t width,
            uint32_t height);
  ~RayCamera() = default;

  Ray SendRay(float u, float v) const;

 private:
  glm::vec3 m_origin;
  glm::vec3 m_target;
  glm::vec3 m_up;
  glm::mat4 m_proj_inverse;
  glm::mat4 m_view_inverse;
  uint32_t m_width;
  uint32_t m_height;
};
