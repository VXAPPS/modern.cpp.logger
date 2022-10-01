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
#include <iterator>
#include <source_location.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief vx (VX APPS) namespace.
 */
namespace vx {

  /**
   * @brief The Severity enum.
   */
  enum class Severity {

    Verbose, /**< Verbose level. */
    Debug,   /**< Debug level. */
    Info,    /**< Info level. */
    Warning, /**< Warning level. */
    Error,   /**< Error level. */
    Fatal    /**< Fatal error level. */
  };

#if defined( LOGGINGALL ) || defined( LOGGINGVERBOSE )
  /** Avoid log below verbose. */
  constexpr Severity avoidLogBelow = Severity::Verbose;
#elif defined( LOGGINGDEBUG )
  /** Avoid log below debug. */
  constexpr Severity avoidLogBelow = Severity::Debug;
#elif defined( LOGGINGWARNING )
  /** Avoid log below warning. */
  constexpr Severity avoidLogBelow = Severity::Warning;
#elif defined( LOGGINGERROR )
  /** Avoid log below error. */
  constexpr Severity avoidLogBelow = Severity::Error;
#elif defined( LOGGINGFATAL )
  /** Avoid log below fatal. */
  constexpr Severity avoidLogBelow = Severity::Fatal;
#elif defined( LOGGINGNONE )
  /** Avoid log below fatal + 1. */
  constexpr Severity avoidLogBelow = Severity::Fatal + 1;
#else
  /** Avoid log below info. */
  constexpr Severity avoidLogBelow = Severity::Info;
#endif

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
    explicit Logger( const std::unordered_map<std::string, std::string> &_configuration ) noexcept;

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
    virtual void log( std::string_view _message,
                      Severity _severity,
                      const std::source_location &_location = std::source_location::current() ) noexcept;

    /**
     * @brief Output the log message.
     * @param _message   Message to log.
     */
    virtual void log( std::string_view _message ) noexcept;
  };
}
