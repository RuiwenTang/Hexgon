#include <GLFW/glfw3.h>

#include <hexgon/core/log.hpp>
#include <hexgon/core/window.hpp>
#include <hexgon/event/window_event.hpp>

namespace Hexgon {

static void glfw_error_callback(int error, const char* description) {
  HEX_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

struct WindowData {
  std::string title;
  uint32_t width;
  uint32_t height;
  bool vsync;

  Window::EventCallbackFn callback;
};

class WindowImpl : public Window {
 public:
  WindowImpl(const WindowProps& props);

  ~WindowImpl() override;

  void OnUpdate() override;

  uint32_t GetWidth() const override { return m_data.width; }
  uint32_t GetHeight() const override { return m_data.height; }

  void SetEventCallback(const EventCallbackFn& callback) override {
    m_data.callback = callback;
  }

  void SetVSync(bool enabled) override {
    if (enabled) {
      glfwSwapInterval(1);
    } else {
      glfwSwapInterval(0);
    }

    m_data.vsync = enabled;
  }

  bool IsVSync() const override { return m_data.vsync; }

  void* GetNativeWindow() const override { return m_nativeWindow; }

 private:
  void Init(const WindowProps& props);
  void Shutdown();

 private:
  GLFWwindow* m_nativeWindow = nullptr;
  WindowData m_data = {};
};

void WindowImpl::OnUpdate() { glfwPollEvents(); }

void WindowImpl::Init(const WindowProps& props) {
  m_data.title = props.title;
  m_data.width = props.width;
  m_data.height = props.height;

  auto ret = glfwInit();

  if (!ret) {
    HEX_CORE_ERROR("Failed init window");
    exit(-1);
  }

  glfwSetErrorCallback(glfw_error_callback);

  m_nativeWindow = glfwCreateWindow(m_data.width, m_data.height,
                                    m_data.title.c_str(), nullptr, nullptr);

  glfwSetWindowUserPointer(m_nativeWindow, &m_data);

  SetVSync(true);

  glfwSetWindowCloseCallback(m_nativeWindow, [](GLFWwindow* window) {
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data->callback(event);
  });
}

void WindowImpl::Shutdown() {
  glfwDestroyWindow(m_nativeWindow);
  glfwTerminate();
}

WindowImpl::WindowImpl(const WindowProps& props) { Init(props); }

WindowImpl::~WindowImpl() { Shutdown(); }

Scope<Window> Window::Create(const WindowProps& props) {
  return CreateScope<WindowImpl>(props);
}

}  // namespace Hexgon
