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

#include <memory>
#include <vector>

#include "Core/Ray.hpp"

struct HitResult {
  glm::vec3 p = {};
  glm::vec3 normal = {};
  float t = {};
  bool front_face = {};

  void SetFaceNormal(Ray const& ray, glm::vec3 const& outward_normal) {
    front_face = glm::dot(ray.Direction(), outward_normal) < 0;
    this->normal = this->front_face ? outward_normal : -outward_normal;
  }
};

class HEX_API Hittable {
 public:
  virtual ~Hittable() = default;

  virtual bool Hit(Ray const& ray, float t_min, float t_max, HitResult& result) const = 0;
};

class HEX_API HittableList : public Hittable {
 public:
  HittableList() = default;
  ~HittableList() override = default;

  void AddObject(std::shared_ptr<Hittable> object) { m_objects.emplace_back(std::move(object));
  }

  bool Hit(Ray const& ray, float t_min, float t_max, HitResult& result) const override;
 private:
  std::vector<std::shared_ptr<Hittable>> m_objects;
};
