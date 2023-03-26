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

#include "Core/Util.hpp"

glm::vec3 Ray::At(float t) const { return m_orig + t * m_dir; }

RayCamera::RayCamera(glm::vec3 const& origin, glm::vec3 const& target, glm::vec3 const& up, float fov, uint32_t width,
                     uint32_t height)
    : m_origin(origin), m_target(target), m_up(up), m_width(width), m_height(height) {
  auto proj = glm::perspective(fov, (float)width / height, 0.01f, glm::length(m_target - m_origin));

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

RayCamera2::RayCamera2(glm::vec3 const& origin, glm::vec3 const& target, glm::vec3 const& up, float fov, uint32_t width,
                       uint32_t height, float aperture, float focus_dist)
    : m_origin(origin), m_target(target), m_up(up), m_width(width), m_height(height) {
  float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

  auto theta = glm::radians(fov);
  auto h = glm::tan(theta / 2.f);
  auto viewport_height = 2.f * h;
  auto viewport_width = aspect * viewport_height;

  auto w = glm::normalize(origin - target);
  m_u = glm::normalize(glm::cross(up, w));
  m_v = glm::cross(w, m_u);

  m_horizontal = focus_dist * viewport_width * m_u;
  m_vertical = focus_dist * viewport_height * m_v;
  m_llc = m_origin - m_horizontal / 2.f - m_vertical / 2.f - focus_dist * w;

  m_lens_radius = aperture / 2.f;
}

Ray RayCamera2::SendRay(float u, float v) const {
  float s = u / (m_width - 1.f);
  float t = v / (m_height - 1.f);

  auto rd = m_lens_radius * Util::RandomInUnit();

  auto offset = m_u * rd.x + m_v * rd.y;

  return Ray{m_origin, m_llc + s + m_horizontal + t * m_vertical - m_origin};
}
