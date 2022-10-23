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

#include "Core/Hittable.hpp"
#include "Core/Ray.hpp"

class HEX_API Material {
 public:
  virtual ~Material() = default;

  virtual bool Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const = 0;
};

class HEX_API Lambertian : public Material {
 public:
  Lambertian(glm::vec3 const& color) : m_albedo(color) {}
  ~Lambertian() override = default;

  bool Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const override;

 private:
  glm::vec3 m_albedo;
};

class HEX_API Metal : public Material {
 public:
  Metal(glm::vec3 const& color) : m_albedo(color) {}
  ~Metal() override = default;

  bool Scatter(Ray const& ray, HitResult const& rec, glm::vec4& attenuation, Ray& scattered) const override;

 private:
  glm::vec3 Reflect(glm::vec3 const& dir, glm::vec3 const& normal) const;

 private:
  glm::vec3 m_albedo;
};
