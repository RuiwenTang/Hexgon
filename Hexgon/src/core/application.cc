#include <filesystem>
#include <hexgon/core/application.hpp>
#include <hexgon/core/log.hpp>

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
}

Application::~Application() { HEX_CORE_INFO("Hexgon engine shutdown"); }

void Application::Close() { m_running = false; }

void Application::Run() {
  while (m_running) {
    m_window->OnUpdate();
  }
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
