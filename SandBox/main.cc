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

using namespace hexgon;

class SimpleLayer : public Layer {
 public:
  SimpleLayer() : Layer("SimpleLayer") {}

  ~SimpleLayer() override = default;

  void OnAttach() override { HEX_INFO("{} layer OnAttach", this->GetLayerName()); }

  void OnDetach() override { HEX_INFO("{} layer OnDetach", this->GetLayerName()); }

  void OnUpdate(float tm) override {}

  void OnEvent(const Event* event) override {
    HEX_INFO("{} layer OnEvent event: {}", this->GetLayerName(), event->GetName());
  }

 private:
};

int main(int argc, const char** argv) {
  Application* app = Application::Create("Hexgon");

  HEX_INFO("create app instance 0x{:x}", reinterpret_cast<uintptr_t>(app));

  app->PushLayer(std::make_shared<SimpleLayer>());

  app->Run();

  return 0;
}
