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
#include "Core/Material.hpp"
#include "Core/Ray.hpp"
#include "Core/Util.hpp"
#include "Layer/GUILayer.hpp"
#include "Layer/RenderLayer.hpp"
#include "glm/common.hpp"

class SimpleRender : public RenderLayer::Renderer {
 public:
  SimpleRender() = default;
  ~SimpleRender() override = default;

  void InitViewPort(uint32_t width, uint32_t height) override {
    m_aspect = width / (float)height;

    auto material_ground = std::make_shared<Lambertian>(glm::vec3{0.8f, 0.8f, 0.f});
    auto material_center = std::make_shared<Lambertian>(glm::vec3{0.1f, 0.2f, 0.5f});
    // auto material_left = std::make_shared<Metal>(glm::vec3{0.8f, 0.8f, 0.8f}, 0.3f);
    // auto material_center = std::make_shared<Dielectric>(1.5f);
    auto material_left = std::make_shared<Dielectric>(1.7f);
    auto material_right = std::make_shared<Metal>(glm::vec3{0.8f, 0.6f, 0.2f}, 0.f);

    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{0.f, 0.f, -1.f}, 0.5f, material_center));
    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{-1.f, 0.f, -1.f}, 0.5f, material_left));
    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{1.f, 0.f, -1.f}, 0.5f, material_right));
    m_objects.AddObject(std::make_shared<Sphere>(glm::vec3{0.f, -100.5f, -1.f}, 100.f, material_ground));
  }

  void DoRender(hexgon::io::Image* image) override {
    int32_t width = image->GetWidth();
    int32_t height = image->GetHeight();
    int32_t samples_per_pixel = 50;

    glm::vec3 origin = glm::vec3(0.f, 0.f, 1.f);

    RayCamera camera{origin, image->GetWidth(), image->GetHeight()};

    float total = width * (float)height;
    float current = 0.f;

    for (int32_t j = height - 1; j >= 0; j--) {
      for (int32_t i = 0; i < width; i++) {
        glm::vec4 color{};
        for (int32_t s = 0; s < samples_per_pixel; s++) {
          float s_x = glm::linearRand(0.f, 1.f);
          float s_y = glm::linearRand(0.f, 1.f);
          float u = glm::clamp(i + s_x, 0.f, (float)width);
          float v = glm::clamp(j + s_y, 0.f, (float)height);
          auto ray = camera.SendRay(u, v);
          color += RayColor(ray, 10);
        }
        color /= (float)samples_per_pixel;
        color = glm::sqrt(color);
        glm::vec4 tmp{
            glm::sqrt(color.r),
            glm::sqrt(color.g),
            glm::sqrt(color.b),
            1.f,
        };
        color = glm::clamp(tmp, glm::vec4{0.f, 0.f, 0.f, 0.f}, glm::vec4{1.f, 1.f, 1.f, 1.f});

        image->PutPixel(i, j, ToRGBA(color));

        current += 1.f;
        UpdateProgress(current / total);
      }
    }
  }

 private:
  glm::vec4 RayColor(Ray const& ray, int32_t depth) {
    if (depth <= 0) {
      return glm::vec4{0.f, 0.f, 0.f, 1.f};
    }

    HitResult result{};

    if (m_objects.Hit(ray, 0.001f, std::numeric_limits<float>::max(), result)) {
      glm::vec4 attenuation{};
      Ray scattered{};
      if (result.material->Scatter(ray, result, attenuation, scattered)) {
        return attenuation * RayColor(scattered, depth - 1);
      } else {
        return glm::vec4{};
      }
    }

    glm::vec3 unit_dir = glm::normalize(ray.Direction());

    auto t = 0.5f * (unit_dir.y + 1.f);

    return glm::mix(glm::vec4{1.f, 1.f, 1.f, 1.f}, glm::vec4{0.5f, 0.7f, 1.f, 1.f}, t);
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
