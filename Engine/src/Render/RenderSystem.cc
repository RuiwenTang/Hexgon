/*
 *   Copyright (c) 2023 RuiwenTang
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

#include <Hexgon/Render/RenderSystem.hpp>

#include "LogPrivate.hpp"

#if defined(HEX_PLATFORM_WINDOWS) || defined(HEX_PLATFORM_LINUX)
#include "Render/Vulkan/RenderSystemVk.hpp"
#endif

namespace hexgon {

std::unique_ptr<RenderSystem> RenderSystem::Init(RenderAPI api, Window* window, bool debug) {
#if defined(HEX_PLATFORM_WINDOWS) || defined(HEX_PLATFORM_LINUX)
  if (api == RenderAPI::kMetal) {
    HEX_CORE_ERROR("Not support Metal API!");
    return std::unique_ptr<RenderSystem>();
  }

  return RenderSystemVk::Init(window, debug);
#else
#error "Not Support Platform"
#endif
}

}  // namespace hexgon
