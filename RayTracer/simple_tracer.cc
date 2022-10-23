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

#include <Hexgon/Hexgon.hpp>
#include <glm/gtc/random.hpp>
#include <limits>

#include "Common/Sphere.hpp"
#include "Core/Hittable.hpp"
#include "Core/Ray.hpp"
#include "Layer/GUILayer.hpp"
#include "Layer/RenderLayer.hpp"

class SimpleRender : public RenderLayer::Renderer {
 public:
  SimpleRender() = default;
  ~SimpleRender() override = default;

  void InitViewPort(uint32_t width, uint32_t height) override {
    m_aspect = width / (float)height;

    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{0.f, 0.f, -1.f}, 0.5f));
    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{0.f, -100.5f, -1.f}, 100.f));
  }

  void DoRender(hexgon::io::Image* image) override {
    int32_t width = image->GetWidth();
    int32_t height = image->GetHeight();
    int32_t samples_per_pixel = 20;

    glm::vec3 origin = glm::vec3(0.f, 0.f, 1.f);

    RayCamera camera{origin, image->GetWidth(), image->GetHeight()};

    float total = width * (float)height;
    float current = 0.f;

    for (int32_t j = height - 1; j >= 0; j--) {
      for (int32_t i = 0; i < width; i++) {
        glm::vec4 color{};
        for (int32_t s = 0; s < samples_per_pixel; s++) {
          float s_x = glm::linearRand(-1.f, 1.f);
          float s_y = glm::linearRand(-1.f, 1.f);
          float u = glm::clamp(i + s_x, 0.f, (float)width);
          float v = glm::clamp(j + s_y, 0.f, (float)height);
          color += RayColor(camera.SendRay(u / (float)width, v / (float)height), 10);
        }
        color /= (float)samples_per_pixel;
        color = glm::sqrt(color);
        color = glm::clamp(color, glm::vec4{0.f, 0.f, 0.f, 0.f}, glm::vec4{1.f, 1.f, 1.f, 1.f});

        image->PutPixel(i, j, ToRGBA(color));
        
        current += 1.f;
        UpdateProgress(current / total);
      }
    }
  }

 private:
  glm::vec3 RandomInUnit() {
    while (true) {
      auto p = glm::linearRand(glm::vec3{-1, -1, -1}, glm::vec3{1, 1, 1});

      if (glm::length(p) >= 1.f) continue;

      return p;
    }
  }

  glm::vec3 UnitRandomInUnit() { return glm::normalize(RandomInUnit()); }

  glm::vec4 RayColor(Ray const& ray, int32_t depth) {
    if (depth <= 0) {
      return glm::vec4{0.f, 0.f, 0.f, 1.f};
    }

    HitResult result{};

    if (m_objects.Hit(ray, 0.f, std::numeric_limits<float>::max(), result)) {
      auto target = result.p + result.normal + UnitRandomInUnit();

      return 0.5f * RayColor(Ray{result.p, target - result.p}, depth - 1);
    }

    glm::vec3 unit_dir = glm::normalize(ray.Direction());

    auto t = 0.5f * (unit_dir.y + 1.f);

    return (1.0f - t) * glm::vec4(1, 1, 1, 1) + t * glm::vec4(0.5f, 0.7f, 1.f, 1.f);
  }

 private:
  float m_aspect = 1.f;
  HittableList m_objects = {};
};

int main(int argc, const char** argv) {
  auto app = hexgon::Application::Create("Simple Ray Tracer");

  HEX_INFO("create app instance 0x{:x}", reinterpret_cast<uintptr_t>(app));

  auto renderer = std::make_unique<SimpleRender>();

  app->GetWindow()->SetClearColor(glm::vec4{0.f, 0.f, 0.f, 0.f});

  auto render_layer = std::make_shared<RenderLayer>();
  render_layer->SetRenderer(renderer.get());

  auto gui_layer = std::make_shared<GUILayer>();

  gui_layer->SetCallback(render_layer.get());

  app->PushLayer(render_layer);

  app->PushLayer(gui_layer);

  app->Run();

  return 0;
}
