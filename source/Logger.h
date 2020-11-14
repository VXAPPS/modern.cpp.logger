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
#include <source_location.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * @brief vx (VX Apps) namespace.
 */
namespace vx {

  /**
   * @brief The Severity enum.
   */
  enum class Severity {

    Verbose, /**< Verbose level. */
    Debug, /**< Debug level. */
    Info, /**< Info level. */
    Warning, /**< Warning level. */
    Error, /**< Error level. */
    Fatal /**< Fatal error level. */
  };

#if defined(LOGGINGALL) || defined(LOGGINGVERBOSE)
  constexpr Severity avoidLogBelow = Severity::Verbose;
#elif defined(LOGGINGDEBUG)
  constexpr Severity avoidLogBelow = Severity::Debug;
#elif defined(LOGGINGWARNING)
  constexpr Severity avoidLogBelow = Severity::Warning;
#elif defined(LOGGINGERROR)
  constexpr Severity avoidLogBelow = Severity::Error;
#elif defined(LOGGINGFATAL)
  constexpr Severity avoidLogBelow = Severity::Fatal;
#elif defined(LOGGINGNONE)
  constexpr Severity avoidLogBelow = Severity::Fatal + 1;
#else
  constexpr Severity avoidLogBelow = Severity::Info;
#endif

  /**
   * @brief Create thread-safe timestamp.
   * @return Timestamp as 'Y-m-dThh:mm:ss.xxxxxx'
   */
  inline std::string timestamp() {

    /* get a precise timestamp as a string */
    struct std::tm currentLocalTime {};
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t( now );
    const auto nowMs = std::chrono::duration_cast<std::chrono::microseconds>( now.time_since_epoch() ) % 1000000;

#ifdef _WIN32
    localtime_s( &currentLocalTime, &nowAsTimeT );
#else
    localtime_r( &nowAsTimeT, &currentLocalTime );
#endif

    std::ostringstream nowSs;
    nowSs
        << std::put_time( &currentLocalTime, "%Y-%m-%dT%T" )
        << '.' << std::setfill( '0' ) << std::setw( 6 ) << nowMs.count()
        << std::put_time( &currentLocalTime, "%z" );
    std::string result = nowSs.str();
    /* somewhat special - maybe see systemtimeformatter */
    result.replace( result.end() - 2, result.end() - 2, ":" );
    return result;
  }

  /**
   * @brief The Logger class.
   * @note Not pure virtual to use as /dev/null logger.
   * @author Florian Becker <fb\@vxapps.com> (VX APPS)
   */
  class Logger {

  public:
    /**
     * @brief Deletet default constructor for Logger.
     */
    Logger() = delete;

    /**
     * @brief Default constructor for Logger.
     * @param _configuration   Logger configuration.
     */
    explicit Logger( const std::unordered_map<std::string, std::string> &_configuration );

    /**
     * @brief Default destructor for Logger.
     */
    virtual ~Logger() = default;

    /**
     * @brief Build the log message.
     * @param _message   Message to log.
     * @param _severity   Severity level of the message.
     * @param _location   Source location information.
     */
    virtual void log( const std::string &_message,
                      const Severity _severity,
                      const nostd::source_location &_location = nostd::source_location::current() );

    /**
     * @brief Output the log message.
     * @param _message   Message to log.
     */
    virtual void log( const std::string &_message );
  };
}
