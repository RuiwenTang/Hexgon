#pragma once

#include <hexgon/core/base.hpp>
#include <hexgon/event/event.hpp>
#include <string>

namespace Hexgon {

struct WindowProps {
  std::string title;
  uint32_t width;
  uint32_t height;

  WindowProps(const std::string& title = "Hexgon Engine", uint32_t width = 1600,
              uint32_t height = 900)
      : title(title), width(width), height(height) {}
};

class Window {
 public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window() = default;

  virtual void OnUpdate() = 0;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  // Window attributes
  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void* GetNativeWindow() const = 0;

  static Scope<Window> Create(const WindowProps& props = WindowProps());
};

}  // namespace Hexgon
