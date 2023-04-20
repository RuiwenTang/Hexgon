
#include <hexgon/core/log.hpp>
#include <hexgon/core/platform.hpp>
#include <hexgon/core/window.hpp>
#include <hexgon/render/render_system.hpp>

#ifdef HEX_PLATFORM_MACOS
#include "src/render/metal/render_system_mtl.hpp"
#endif

namespace Hexgon {

Scope<RenderSystem> RenderSystem::LoadRenderSystem(RenderAPI API) {
#ifdef HEX_PLATFORM_MACOS
  HEX_CORE_INFO("Pick Metal RenderSystem");
  return LoadMetalRender();
#endif
}

}  // namespace Hexgon
