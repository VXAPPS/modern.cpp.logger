/*
 * Copyright (c) 2020 Florian Becker <fb@vxapps.com> (VX APPS).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

/* stl header */
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * @brief vx (VX Apps) namespace.
 */
namespace vx {

  /* the log levels we support */
  enum class Severity {

    Verbose,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
  };

  /* all, something in between, none or default to info */
#if defined(LOGGINGALL) || defined(LOGGINGVERBOSE)
  constexpr Severity avoidLogAbove = Severity::Verbose;
#elif defined(LOGGINGDEBUG)
  constexpr Severity avoidLogAbove = Severity::Debug;
#elif defined(LOGGINGWARNING)
  constexpr Severity avoidLogAbove = Severity::Warning;
#elif defined(LOGGINGERROR)
  constexpr Severity avoidLogAbove = Severity::Error;
#elif defined(LOGGINGFATAL)
  constexpr Severity avoidLogAbove = Severity::Fatal;
#elif defined(LOGGINGNONE)
  constexpr Severity avoidLogAbove = Severity::Fatal + 1;
#else
  constexpr Severity avoidLogAbove = Severity::Info;
#endif

  /* returns formated to: 'Y-m-dTh:m:s.xxxxxx' */
  inline std::string timestamp() {

    /* get a precise timestamp as a string */
    struct std::tm currentLocalTime {};
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t( now );
    const auto nowMs = std::chrono::duration_cast<std::chrono::microseconds>( now.time_since_epoch() ) % 1000000;

    std::ostringstream nowSs;
    nowSs
        << std::put_time( localtime_r( &nowAsTimeT, &currentLocalTime ), "%Y-%m-%dT%T" )
        << '.' << std::setfill( '0' ) << std::setw( 6 ) << nowMs.count()
        << std::put_time( localtime_r( &nowAsTimeT, &currentLocalTime ), "%z" );
    std::string result = nowSs.str();
    /* somewhat special - maybe see systemtimeformatter */
    result.replace( result.end() - 2, result.end() - 2, ":" );
    return result;
  }

  inline std::string threadId() {

    std::ostringstream s;
    s << " [" << std::this_thread::get_id() << "]";
    s.flush();
    std::string result = s.str();
    return result;
  }

  /* logger base class, not pure virtual so you can use as a null logger if you want */
  class Logger {

  public:
    Logger() = delete;
    explicit Logger( const std::unordered_map<std::string, std::string> &_configuration );
    virtual ~Logger() = default;
    virtual void log( const std::string &_message, const Severity _severity );
    virtual void log( const std::string &_message );
  };
}
