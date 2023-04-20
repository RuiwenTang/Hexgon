#pragma once

#include <hexgon/core/base.hpp>

namespace Hexgon {

class Window;

enum class RenderAPI {
  kNone,
  kOpenGL,
  kVulkan,
  kMetal,
};

class RenderSystem {
 public:
  virtual ~RenderSystem() = default;

  static Scope<RenderSystem> LoadRenderSystem(RenderAPI API);

  virtual bool Init() = 0;

  virtual void Shutdown() = 0;
};

}  // namespace Hexgon
