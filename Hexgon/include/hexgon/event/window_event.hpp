#pragma once

#include <hexgon/event/event.hpp>

namespace Hexgon {

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;
  ~WindowCloseEvent() override = default;

  static EventType GetStaticType() { return EventType::WindowClose; }

  EventType GetEventType() const override { return EventType::WindowClose; }

  const char* GetName() const override { return "WindowClose"; }

  int GetCategoryFlags() const override { return EventCategoryApplication; }
};

}  // namespace Hexgon
