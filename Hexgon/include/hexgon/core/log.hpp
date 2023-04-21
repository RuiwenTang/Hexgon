#pragma once

#include <hexgon/core/base.hpp>

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Hexgon {

class Log {
 public:
  static void Init();

  static Ref<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
  static Ref<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

 private:
  static Ref<spdlog::logger> s_coreLogger;
  static Ref<spdlog::logger> s_clientLogger;
};

}  // namespace Hexgon

// Core log macros
#define HEX_CORE_TRACE(...) ::Hexgon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HEX_CORE_INFO(...) ::Hexgon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HEX_CORE_WARN(...) ::Hexgon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HEX_CORE_ERROR(...) ::Hexgon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HEX_CORE_CRITICAL(...) \
  ::Hexgon::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HEX_TRACE(...) ::Hexgon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HEX_INFO(...) ::Hexgon::Log::GetClientLogger()->info(__VA_ARGS__)
#define HEX_WARN(...) ::Hexgon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HEX_ERROR(...) ::Hexgon::Log::GetClientLogger()->error(__VA_ARGS__)
#define HEX_CRITICAL(...) \
  ::Hexgon::Log::GetClientLogger()->critical(__VA_ARGS__)
