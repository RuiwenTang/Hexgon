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

#include <spdlog/sinks/stdout_color_sinks.h>

#include <Hexgon/Core/Log.hpp>
#include <vector>

namespace hexgon {

Log* Log::GetLogger() {
  static Log* g_instance = nullptr;

  if (!g_instance) {
    g_instance = new Log;
    g_instance->Init();
  }

  return g_instance;
}

void Log::Init() {
  std::vector<spdlog::sink_ptr> log_sinks{};

  log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

  log_sinks[0]->set_pattern("%^[%T] %n: %v%$");

  m_logger = std::make_shared<spdlog::logger>("APP", log_sinks.begin(), log_sinks.end());
  m_logger->set_level(spdlog::level::trace);
  m_logger->flush_on(spdlog::level::trace);

  spdlog::register_logger(m_logger);
}

}  // namespace hexgon
