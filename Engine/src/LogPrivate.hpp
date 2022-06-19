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

#ifndef ENGINE_SRC_LOG_PRIVATE_HPP_
#define ENGINE_SRC_LOG_PRIVATE_HPP_

#include <spdlog/spdlog.h>

namespace hexgon {

class LogPrivate final {
 public:
  LogPrivate() = default;
  ~LogPrivate() = default;

  static LogPrivate* GetLogger();

  void Init();

  spdlog::logger* Logger() const { return m_logger.get(); }

 private:
  std::shared_ptr<spdlog::logger> m_logger = nullptr;
};

}  // namespace hexgon

#define HEX_CORE_TRACE(...) ::hexgon::LogPrivate::GetLogger()->Logger()->trace(__VA_ARGS__)
#define HEX_CORE_INFO(...) ::hexgon::LogPrivate::GetLogger()->Logger()->info(__VA_ARGS__)
#define HEX_CORE_WARN(...) ::hexgon::LogPrivate::GetLogger()->Logger()->warn(__VA_ARGS__)
#define HEX_CORE_ERROR(...) ::hexgon::LogPrivate::GetLogger()->Logger()->error(__VA_ARGS__)
#define HEX_CORE_CRITICAL(...) ::hexgon::LogPrivate::GetLogger()->Logger()->critical(__VA_ARGS__)

#endif  // ENGINE_SRC_LOG_PRIVATE_HPP_
