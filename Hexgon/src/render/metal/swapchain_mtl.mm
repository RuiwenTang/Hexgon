#include "src/render/metal/swapchain_mtl.h"
#include "hexgon/render/swapchain.hpp"

namespace Hexgon {

SwapchainMTL::SwapchainMTL(const SwapchainDescriptor& desc, MTKView* view)
    : Swapchain(desc), m_view(view) {}

SwapchainMTL::~SwapchainMTL() {}

bool SwapchainMTL::Init() { return true; }

void SwapchainMTL::Shutdown() {}

glm::ivec2 SwapchainMTL::GetResolution() {
  return glm::ivec2{m_view.frame.size.width, m_view.frame.size.height};
}

void SwapchainMTL::Present() { [m_view draw]; }

}