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

#include "Core/Material.hpp"

#include "Core/Util.hpp"

bool Lambertian::Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const {
  auto scatter_dir = rec.normal + Util::UnitRandomInUnit();

  if (Util::NearZero(scatter_dir)) {
    scatter_dir = rec.normal;
  }

  scattered = Ray{rec.p, scatter_dir};

  attenuation = glm::vec4{m_albedo, 1.f};

  return true;
}

bool Metal::Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const {
  auto reflected = Reflect(ray.Direction(), rec.normal);

  scattered = Ray{rec.p, reflected + m_fuzz * Util::UnitRandomInUnit()};
  attenuation = glm::vec4{m_albedo, 1.f};

  return (glm::dot(scattered.Direction(), rec.normal) > 0.f);
}

glm::vec3 Metal::Reflect(glm::vec3 const& dir, glm::vec3 const& normal) const { return glm::reflect(dir, normal); }

bool Dielectric::Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const {
  attenuation = glm::vec4{1.f, 1.f, 1.f, 1.f};

  float reflect_radios = rec.front_face ? (1.f / m_ir) : m_ir;

  auto cos_theta = glm::min(glm::dot(-ray.Direction(), rec.normal), 1.0f);
  auto sin_theta = glm::sqrt(1.f - cos_theta * cos_theta);

  bool cannot_refract = reflect_radios * sin_theta > 1.0f;

  glm::vec3 refrected;
  if (cannot_refract) {
    refrected = glm::reflect(ray.Direction(), rec.normal);
  } else {
    refrected = this->Refract(ray.Direction(), rec.normal, reflect_radios);
  }
  scattered = Ray{rec.p, refrected};

  return true;
}

glm::vec3 Dielectric::Refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) const {
  auto cos_theta = glm::min(glm::dot(-uv, n), 1.0f);
  glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  glm::vec3 r_out_parallel = -glm::sqrt(glm::abs(1.0f - glm::length(r_out_perp))) * n;
  return r_out_perp + r_out_parallel;
}
