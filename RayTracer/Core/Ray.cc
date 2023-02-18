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

#include "Core/Ray.hpp"

glm::vec3 Ray::At(float t) const { return m_orig + t * m_dir; }

RayCamera::RayCamera(glm::vec3 const& origin, glm::vec3 const& target, glm::vec3 const& up, float fov, uint32_t width,
                     uint32_t height)
    : m_origin(origin), m_target(target), m_up(up), m_width(width), m_height(height) {
  auto proj = glm::perspective(fov, (float)width / height, 0.01f, 1000.f);

  m_proj_inverse = glm::inverse(proj);

  auto view = glm::lookAt(m_origin, m_target, m_up);

  m_view_inverse = glm::inverse(view);
}

Ray RayCamera::SendRay(float u, float v) const {
  glm::vec2 coord{u / m_width, v / m_height};

  coord = coord * 2.f - 1.f;

  auto target = m_proj_inverse * glm::vec4{coord, 1.f, 1.f};

  auto dir = glm::vec3(m_view_inverse * glm::vec4{glm::normalize(glm::vec3(target) / target.w), 0.f});


  return Ray{m_origin, glm::normalize(glm::vec3{dir})};
}
