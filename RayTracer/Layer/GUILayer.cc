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

#include "Layer/GUILayer.hpp"

void GUILayer::OnImguiInit() {
  auto scale = GetApplication()->GetWindow()->GetDisplayScale();

  ImGuiIO& io = ImGui::GetIO();
  ImFontConfig config{};

  config.OversampleH = scale.x;
  config.OversampleV = scale.y;

  io.Fonts->AddFontDefault(&config);
}

void GUILayer::OnDrawImgui(float tm) {
  ImGui::Begin("Control Panel");

  if (ImGui::Button("Render")) {
    if (m_callback) {
      m_callback->OnRender();
    }
  }

  ImGui::ProgressBar(m_progress);

  ImGui::Text("Frame Time: %d ms", 0);

  ImGui::End();
}
