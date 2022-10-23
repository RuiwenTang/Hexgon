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

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>


namespace Util {

static inline glm::vec3 RandomInUnit() {
  while (true) {
    auto p = glm::linearRand(glm::vec3{-1, -1, -1}, glm::vec3{1, 1, 1});

    if (glm::length(p) >= 1.f) continue;

    return p;
  }
}

static inline glm::vec3 UnitRandomInUnit() { return glm::normalize(RandomInUnit()); }

static inline glm::vec3 RandomInHemiSphere(glm::vec3 const& normal) {
  auto p = UnitRandomInUnit();
  if (glm::dot(p, normal) > 0.f) {
    return p;
  } else {
    return -p;
  }
}

static inline bool NearZero(glm::vec3 const& value) {
  float s = 1e-8;
  return (glm::abs(value[0]) < s) && (glm::abs(value[1]) < s) && (glm::abs(value[2]) < s);
}

}  // namespace Util