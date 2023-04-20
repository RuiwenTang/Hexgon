#pragma once

#include <hexgon/event/event.hpp>

namespace Hexgon {

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;
  ~WindowCloseEvent() override = default;

  EventType GetEventType() const override { return EventType::WindowClose; }

  const char* GetName() const override { return "WindowClose"; }

  int GetCategoryFlags() const override { return EventCategoryApplication; }
};

class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : m_width(width), m_height(height) {}
  ~WindowResizeEvent() override = default;

  EventType GetEventType() const override { return EventType::WindowResize; }

  const char* GetName() const override { return "WindowResize"; }

  int GetCategoryFlags() const override { return EventCategoryApplication; }

  uint32_t GetWidth() const { return m_width; }
  uint32_t GetHeight() const { return m_height; }

 private:
  uint32_t m_width;
  uint32_t m_height;
};

}  // namespace Hexgon
