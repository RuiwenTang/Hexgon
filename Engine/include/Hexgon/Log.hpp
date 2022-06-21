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

#ifndef ENGINE_INCLUDE_HEXGON_LOG_HPP_
#define ENGINE_INCLUDE_HEXGON_LOG_HPP_

#include <spdlog/spdlog.h>

#include <Hexgon/Macro.hpp>

namespace hexgon {

class HEX_API Log final {
 public:
  ~Log() = default;

  static Log* GetLogger();

  spdlog::logger* Logger() const { return m_logger.get(); }

 private:
  Log() = default;
  void Init();

 private:
  std::shared_ptr<spdlog::logger> m_logger = nullptr;
};

}  // namespace hexgon

#define HEX_TRACE(...) ::hexgon::Log::GetLogger()->Logger()->trace(__VA_ARGS__)
#define HEX_INFO(...) ::hexgon::Log::GetLogger()->Logger()->info(__VA_ARGS__)
#define HEX_WARN(...) ::hexgon::Log::GetLogger()->Logger()->warn(__VA_ARGS__)
#define HEX_ERROR(...) ::hexgon::Log::GetLogger()->Logger()->error(__VA_ARGS__)
#define HEX_CRITICAL(...) ::hexgon::Log::GetLogger()->Logger()->critical(__VA_ARGS__)

#endif  // ENGINE_INCLUDE_HEXGON_LOG_HPP_
