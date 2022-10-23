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

  scattered = Ray{rec.p, scatter_dir};

  if (Util::NearZero(scatter_dir)) {
    scatter_dir = rec.normal;
  }

  attenuation = glm::vec4{m_albedo, 1.f};

  return true;
}

bool Metal::Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const {
  auto reflected = Reflect(ray.Direction(), rec.normal);

  scattered = Ray{rec.p, reflected};
  attenuation = glm::vec4{m_albedo, 1.f};

  return (glm::dot(scattered.Direction(), rec.normal) > 0.f);
}

glm::vec3 Metal::Reflect(glm::vec3 const& dir, glm::vec3 const& normal) const {
  return dir - 2.f * glm::dot(dir, normal) * normal;
}
