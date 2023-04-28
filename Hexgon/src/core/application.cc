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

  m_swapchain = m_renderSystem->CreateSwapchain(m_window.get(), {});

  if (!m_swapchain) {
    HEX_CORE_ERROR("Swapchain created failed!");
    return;
  }

  if (!m_swapchain->Init()) {
    HEX_CORE_ERROR("Swapchain init failed!");
    return;
  }

  m_cmd = m_renderSystem->CreateCommandBuffer({});

  if (!m_cmd) {
    HEX_CORE_ERROR("Failed create main screen command buffer!");
    return;
  }

  ShaderDescriptor desc{};
  desc.entry_point = "vMain";
  desc.type = ShaderType::kVertex;
  desc.sourceType = ShaderSourceType::kCodeFile;
  desc.source = "triangle.metal";

  auto shader = m_renderSystem->CreateShader(desc);

  while (m_running) {
    m_renderSystem->BeginFrame();

    auto render_target = m_swapchain->AcquireNextFrame();

    m_cmd->Begin();

    // clear value
    std::vector<AttachmentClear> clear_values{2};
    clear_values[0].flag = ClearFlags::kColor;
    clear_values[0].value = ClearValue{1.f, 1.f, 0.f, 1.f};
    clear_values[1].flag = ClearFlags::kDepthStencil;
    clear_values[1].value = ClearValue(0.f, 0);

    if (!m_cmd->BeginRenderPass(m_swapchain->GetRenderPass(),
                                render_target.get(), clear_values)) {
      HEX_CORE_ERROR("Failed begin screen render pass!");
      m_running = false;
    }

    m_cmd->EndRenderPass();

    m_cmd->End();

    m_renderSystem->Submit(m_cmd);

    m_swapchain->Present();

    m_renderSystem->EndFrame();

    m_window->OnUpdate();
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
