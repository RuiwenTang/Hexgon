#pragma once

#include <functional>
#include <hexgon/core/window.hpp>
#include <hexgon/event/window_event.hpp>
#include <hexgon/render/render_system.hpp>
#include <hexgon/render/swapchain.hpp>
#include <mutex>
#include <string>
#include <vector>

namespace Hexgon {

struct ApplicationCommandLineArgs {
  int Count = 0;
  char** Args = nullptr;

  const char* operator[](int index) const { return Args[index]; }
};

struct ApplicationSpecification {
  std::string name = "Hexgon Application";
  std::string workingDirectory;
  ApplicationCommandLineArgs commandLineArgs;
};

class Application {
 public:
  explicit Application(const ApplicationSpecification& specification);
  virtual ~Application();

  void Close();

  static Application& Get() { return *s_instance; }

  const ApplicationSpecification& GetSpecification() const {
    return m_specification;
  }

  void SubmitToMainThread(const std::function<void()>& function);

  void Run();

  void OnEvent(Event& event);

 private:
  void ExecuteMainThreadQueue();

  bool OnWindowClose(WindowCloseEvent& e);
  bool OnWindowResize(WindowResizeEvent& e);

 private:
  ApplicationSpecification m_specification;
  bool m_running = true;
  bool m_minimized = false;
  float m_lastFrameTime = 0.0f;
  Scope<Window> m_window = {};
  Scope<RenderSystem> m_renderSystem = {};
  Scope<Swapchain> m_swapchain = {};

  std::vector<std::function<void()>> m_mainThreadQueue;
  std::mutex m_mainThreadQueueMutex;

 private:
  static Application* s_instance;
};

}  // namespace Hexgon
