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

#include "Core/Ray.hpp"
#include "Layer/GUILayer.hpp"
#include "Layer/RenderLayer.hpp"

class SimpleRender : public RenderLayer::Renderer {
 public:
  SimpleRender() = default;
  ~SimpleRender() override = default;

  void InitViewPort(uint32_t width, uint32_t height) override { m_aspect = width / (float)height; }

  void DoRender(hexgon::io::Image* image) override {
    int32_t width = image->GetWidth();
    int32_t height = image->GetHeight();

    glm::vec3 origin = glm::vec3(0.f, 0.f, 1.f);

    glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);

    for (int32_t j = height - 1; j >= 0; j--) {
      for (int32_t i = 0; i < width; i++) {
        glm::vec3 uv = CalculateUV(i / (float)width, j / (float)height);

        Ray ray(origin, uv - origin);

        float t = HitSphere(center, 0.5f, ray);

        if (t > 0.f) {
          glm::vec3 n = glm::normalize(ray.At(t) - center);

          image->PutPixel(i, j, ToRGBA(glm::vec4{0.5f * (n + 1.f), 1.f}));
        } else {
          image->PutPixel(i, j, ToRGBA(RayColor(ray)));
        }
      }
    }
  }

 private:
  glm::vec3 CalculateUV(float x, float y) { return glm::vec3{(x * 2.f - 1.f) * m_aspect, y * 2.f - 1.f, 0.f}; }

  glm::vec4 RayColor(Ray const& ray) {
    glm::vec3 unit_dir = glm::normalize(ray.Direction());

    auto t = 0.5f * (unit_dir.y + 1.f);

    return (1.0f - t) * glm::vec4(1, 1, 1, 1) + t * glm::vec4(0.5f, 0.7f, 1.f, 1.f);
  }

  float HitSphere(glm::vec3 const& center, float radius, Ray const& r) {
    auto oc = r.Origin() - center;

    auto a = glm::dot(r.Direction(), r.Direction());
    auto half_b = glm::dot(oc, r.Direction());
    auto c = glm::dot(oc, oc) - radius * radius;

    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0.f) {
      return -1.f;
    } else {
      return (-half_b - glm::sqrt(discriminant)) / a;
    }
  }

 private:
  float m_aspect = 1.f;
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
