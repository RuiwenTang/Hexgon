#include <filesystem>
#include <hexgon/core/application.hpp>
#include <hexgon/core/log.hpp>
#include <hexgon/core/platform.hpp>

#include "hexgon/render/format.hpp"
#include "hexgon/render/renderpass.hpp"

namespace Hexgon {

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
    : m_specification(specification) {
  s_instance = this;

  if (!m_specification.workingDirectory.empty()) {
    std::filesystem::current_path(m_specification.workingDirectory);
  }

  Log::Init();

  HEX_CORE_INFO("Hexgon engine start.");

  m_window = Window::Create(WindowProps{m_specification.name});

  m_window->SetEventCallback(HEX_BIND_EVENT_FN(Application::OnEvent));

#ifdef HEX_PLATFORM_MACOS
  m_renderSystem = RenderSystem::LoadRenderSystem(RenderAPI::kMetal);
#elif defined(HEX_PLATFORM_WINDOWS)
  m_renderSystem = RenderSystem::LoadRenderSystem(RenderAPI::kVulkan);
#endif
}

Application::~Application() { HEX_CORE_INFO("Hexgon engine shutdown"); }

void Application::Close() { m_running = false; }

void Application::Run() {
  if (!m_renderSystem) {
    HEX_CORE_ERROR("Render system not pick!");
    return;
  }

  if (!m_renderSystem->Init()) {
    HEX_CORE_ERROR("Render system init failed!");
    return;
  }

  if (!InitScreenRenderPass()) {
    HEX_CORE_ERROR("Screen RenderPass init failed!");
    return;
  }

  m_swapchain = m_renderSystem->CreateSwapchain(m_window.get(), {},
                                                m_screen_renderpass.get());

  if (!m_swapchain) {
    HEX_CORE_ERROR("Swapchain created failed!");
    return;
  }

  if (!m_swapchain->Init()) {
    HEX_CORE_ERROR("Swapchain init failed!");
    return;
  }

  while (m_running) {
    auto render_target = m_swapchain->AcquireNextFrame();

    m_window->OnUpdate();

    m_swapchain->Present();
  }

  m_swapchain->Shutdown();
  m_renderSystem->Shutdown();
}

void Application::OnEvent(Event& event) {
  HEX_CORE_INFO("receive event: {}", event.ToString());

  EventDispatcher dispatcher{event};

  dispatcher.Dispatch<WindowCloseEvent, EventType::WindowClose>(
      HEX_BIND_EVENT_FN(Application::OnWindowClose));

  dispatcher.Dispatch<WindowResizeEvent, EventType::WindowResize>(
      HEX_BIND_EVENT_FN(Application::OnWindowResize));
}

void Application::SubmitToMainThread(const std::function<void()>& function) {
  std::scoped_lock<std::mutex> lock(m_mainThreadQueueMutex);

  m_mainThreadQueue.emplace_back(function);
}

bool Application::InitScreenRenderPass() {
  auto const& swapchain_desc = m_swapchain->GetDescriptor();

  RenderPassDescriptor desc{};

  // TODO should query swapchain formats
  desc.color_attachments.emplace_back(AttachmentDescriptor{
      Format::RGBA8UNorm,
      AttachmentLoadOp::kClear,
      AttachmentStoreOp::kStore,
  });

  // depth
  desc.depth_attachment = AttachmentDescriptor{
      Format::D24UNormS8UInt,
      AttachmentLoadOp::kClear,
      AttachmentStoreOp::kNotCare,
  };

  // for now not use stencil

  m_screen_renderpass = m_renderSystem->CreateRenderPass(desc);

  return m_screen_renderpass != nullptr;
}

void Application::ExecuteMainThreadQueue() {
  std::scoped_lock<std::mutex> lock(m_mainThreadQueueMutex);

  for (auto& func : m_mainThreadQueue) func();

  m_mainThreadQueue.clear();
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
  m_running = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  if (e.GetWidth() == 0 || e.GetHeight() == 0) {
    m_minimized = true;
    return false;
  }

  m_minimized = false;

  return false;
}

}  // namespace Hexgon
