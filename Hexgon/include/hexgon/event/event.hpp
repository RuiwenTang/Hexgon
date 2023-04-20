#pragma once

#include <string>

namespace Hexgon {

enum class EventType {
  None,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None = 0,
  EventCategoryApplication = 1,
  EventCategoryInput = 1 << 1,
  EventCategoryKeyboard = 1 << 2,
  EventCategoryMouse = 1 << 3,
  EventCategoryMouseButton = 1 << 4,
};

class Event {
 public:
  virtual ~Event() = default;

  virtual EventType GetEventType() const = 0;

  virtual const char* GetName() const = 0;

  virtual int GetCategoryFlags() const = 0;

  virtual std::string ToString() const { return GetName(); }

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }

 public:
  bool handled = false;
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_event(event) {}

  // F will be deduced by the compiler
  template <typename T, EventType Type, typename F>
  bool Dispatch(const F& func) {
    if (m_event.GetEventType() == Type) {
      m_event.handled |= func(static_cast<T&>(m_event));
      return true;
    }
    return false;
  }

 private:
  Event& m_event;
};

}  // namespace Hexgon
