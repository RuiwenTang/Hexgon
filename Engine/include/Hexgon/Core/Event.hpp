/*
 *   Copyright (c) 2022 RuiwenTang
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#ifndef ENGINE_INCLUDE_HEXGON_CORE_EVENT_HPP_
#define ENGINE_INCLUDE_HEXGON_CORE_EVENT_HPP_

#include <Hexgon/Macro.hpp>
#include <string>

namespace hexgon {

namespace KeyCode {

enum Code {
  // From glfw3.h
  Space = 32,
  Apostrophe = 39, /* ' */
  Comma = 44,      /* , */
  Minus = 45,      /* - */
  Period = 46,     /* . */
  Slash = 47,      /* / */

  D0 = 48, /* 0 */
  D1 = 49, /* 1 */
  D2 = 50, /* 2 */
  D3 = 51, /* 3 */
  D4 = 52, /* 4 */
  D5 = 53, /* 5 */
  D6 = 54, /* 6 */
  D7 = 55, /* 7 */
  D8 = 56, /* 8 */
  D9 = 57, /* 9 */

  Semicolon = 59, /* ; */
  Equal = 61,     /* = */

  A = 65,
  B = 66,
  C = 67,
  D = 68,
  E = 69,
  F = 70,
  G = 71,
  H = 72,
  I = 73,
  J = 74,
  K = 75,
  L = 76,
  M = 77,
  N = 78,
  O = 79,
  P = 80,
  Q = 81,
  R = 82,
  S = 83,
  T = 84,
  U = 85,
  V = 86,
  W = 87,
  X = 88,
  Y = 89,
  Z = 90,

  LeftBracket = 91,  /* [ */
  Backslash = 92,    /* \ */
  RightBracket = 93, /* ] */
  GraveAccent = 96,  /* ` */

  World1 = 161, /* non-US #1 */
  World2 = 162, /* non-US #2 */

  /* Function keys */
  Escape = 256,
  Enter = 257,
  Tab = 258,
  Backspace = 259,
  Insert = 260,
  Delete = 261,
  Right = 262,
  Left = 263,
  Down = 264,
  Up = 265,
  PageUp = 266,
  PageDown = 267,
  Home = 268,
  End = 269,
  CapsLock = 280,
  ScrollLock = 281,
  NumLock = 282,
  PrintScreen = 283,
  Pause = 284,
  F1 = 290,
  F2 = 291,
  F3 = 292,
  F4 = 293,
  F5 = 294,
  F6 = 295,
  F7 = 296,
  F8 = 297,
  F9 = 298,
  F10 = 299,
  F11 = 300,
  F12 = 301,
  F13 = 302,
  F14 = 303,
  F15 = 304,
  F16 = 305,
  F17 = 306,
  F18 = 307,
  F19 = 308,
  F20 = 309,
  F21 = 310,
  F22 = 311,
  F23 = 312,
  F24 = 313,
  F25 = 314,

  /* Keypad */
  KP0 = 320,
  KP1 = 321,
  KP2 = 322,
  KP3 = 323,
  KP4 = 324,
  KP5 = 325,
  KP6 = 326,
  KP7 = 327,
  KP8 = 328,
  KP9 = 329,
  KPDecimal = 330,
  KPDivide = 331,
  KPMultiply = 332,
  KPSubtract = 333,
  KPAdd = 334,
  KPEnter = 335,
  KPEqual = 336,

  LeftShift = 340,
  LeftControl = 341,
  LeftAlt = 342,
  LeftSuper = 343,
  RightShift = 344,
  RightControl = 345,
  RightAlt = 346,
  RightSuper = 347,
  Menu = 348
};

}

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
  MouseScrolled,
  CharEvent,
};

enum MouseCode {
  Button0 = 0,
  Button1 = 1,
  Button2 = 2,
  Button3 = 3,
  Button4 = 4,
  Button5 = 5,
  Button6 = 6,
  Button7 = 7,

  ButtonLast = Button7,
  ButtonLeft = Button0,
  ButtonRight = Button1,
  ButtonMiddle = Button2
};

class HEX_API Event {
 public:
  virtual ~Event() = default;

  virtual EventType GetType() const = 0;
  virtual std::string GetName() const = 0;
};

class HEX_API KeyEvent : public Event {
 public:
  KeyEvent(KeyCode::Code code) : m_code(code) {}

  ~KeyEvent() override = default;

  KeyCode::Code GetKeyCode() const { return m_code; }

 private:
  KeyCode::Code m_code;
};

class HEX_API KeyPressEvent : public KeyEvent {
 public:
  KeyPressEvent(KeyCode::Code code);
  ~KeyPressEvent() override = default;

  std::string GetName() const override;

  EventType GetType() const override;
};

class HEX_API KeyReleaseEvent : public KeyEvent {
 public:
  KeyReleaseEvent(KeyCode::Code code);
  ~KeyReleaseEvent() override = default;

  std::string GetName() const override;

  EventType GetType() const override;
};

class MouseEvent : public Event {
 public:
  MouseEvent(float x, float y) : m_mouse_x(x), m_mouse_y(y) {}
  ~MouseEvent() override = default;

  float GetX() const { return m_mouse_x; }
  float GetY() const { return m_mouse_y; }

 private:
  float m_mouse_x;
  float m_mouse_y;
};

class MouseMovedEvent : public MouseEvent {
 public:
  MouseMovedEvent(float x, float y) : MouseEvent(x, y) {}
  ~MouseMovedEvent() override = default;

  EventType GetType() const override { return EventType::MouseMoved; }
  std::string GetName() const override { return std::string("MouseMoved"); }
};

class MouseScrolledEvent : public MouseEvent {
 public:
  MouseScrolledEvent(float x, float y, float offset_x, float offset_y)
      : MouseEvent(x, y), m_offset_x(offset_x), m_offset_y(offset_y) {}
  ~MouseScrolledEvent() override = default;

  EventType GetType() const override { return EventType::MouseScrolled; }

  std::string GetName() const override { return std::string("MouseScrolled"); }

  float GetOffsetX() const { return m_offset_x; }
  float GetOffsetY() const { return m_offset_y; }

 private:
  float m_offset_x;
  float m_offset_y;
};

class MousePressedEvent : public MouseEvent {
 public:
  MousePressedEvent(float x, float y, MouseCode code) : MouseEvent(x, y), m_code(code) {}
  ~MousePressedEvent() override = default;

  EventType GetType() const override { return EventType::MouseButtonPressed; }

  std::string GetName() const override { return std::string("MouseButtonPressed"); }

  MouseCode GetCode() const { return m_code; }

 private:
  MouseCode m_code;
};

class MouseReleasedEvent : public MouseEvent {
 public:
  MouseReleasedEvent(float x, float y, MouseCode code) : MouseEvent(x, y), m_code(code) {}
  ~MouseReleasedEvent() override = default;

  EventType GetType() const override { return EventType::MouseButtonReleased; }

  std::string GetName() const override { return std::string("MouseButtonReleased"); }

  MouseCode GetCode() const { return m_code; }

 private:
  MouseCode m_code;
};

class CharEvent : public Event {
 public:
  CharEvent(uint32_t c) : m_char(c) {}

  EventType GetType() const override { return EventType::CharEvent; }

  std::string GetName() const override { return std::string("char event"); }

  uint32_t GetChar() const { return m_char; }

 private:
  uint32_t m_char;
};

}  // namespace hexgon

#endif  // ENGINE_INCLUDE_HEXGON_CORE_EVENT_HPP_