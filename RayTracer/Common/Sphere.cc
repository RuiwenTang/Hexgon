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

#include "Common/Sphere.hpp"

bool Sphere::Hit(Ray const& ray, float t_min, float t_max, HitResult& result) const {
  auto oc = ray.Origin() - m_center;

  auto a = glm::dot(ray.Direction(), ray.Direction());
  auto half_b = glm::dot(oc, ray.Direction());
  auto c = glm::dot(oc, oc) - m_radius * m_radius;

  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0.f) {
    return false;
  }

  auto root = (-half_b - glm::sqrt(discriminant)) / a;

  if (root < t_min || root > t_max) {
    return false;
  }

  result.t = root;
  result.p = ray.At(root);
  result.SetFaceNormal(ray, (result.p - m_center) / m_radius);
  result.material = m_material;

  return true;
}
