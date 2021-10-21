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
#include <fstream>
#include <shared_mutex>
#include <string>
#include <unordered_map>

/* local header */
#include "Logger.h"

/**
 * @brief vx (VX APPS) namespace.
 */
namespace vx {

  /**
   * @brief The FileLogger class for writing messages to file.
   * @author Florian Becker <fb\@vxapps.com> (VX APPS)
   */
  class FileLogger : public Logger {

  public:
    /**
     * @brief Deletet default constructor for FileLogger.
     */
    FileLogger() = delete;

    /**
     * @brief Default constructor for FileLogger.
     * @param _configuration   Logger configuration.
     */
    explicit FileLogger( const std::unordered_map<std::string, std::string> &_configuration ) noexcept( false );

    /**
     * @brief Build the log message.
     * @param _message   Message to log.
     * @param _severity   Severity level of the message.
     * @param _location   Source location information.
     */
    void log( std::string_view _message,
              const Severity _severity,
              const nostd::source_location &_location = nostd::source_location::current() ) noexcept override;

    /**
     * @brief Output the log message.
     * @param _message   Message to log.
     */
    void log( std::string_view _message ) noexcept override;

  protected:
    /**
     * @brief Close and reopen the log file.
     */
    void reopen() noexcept;

  private:
    /**
     * @brief Log filename.
     */
    std::string m_filename {};

    /**
     * @brief Log file handle.
     */
    std::ofstream m_file {};

    /**
     * @brief Interval for reopening the log file.
     */
    std::chrono::seconds m_reopenInterval {};

    /**
     * @brief Timestamp of last reopen activity.
     */
    std::chrono::system_clock::time_point m_lastReopen {};

    /**
     * @brief Member for shared mutex.
     */
    mutable std::shared_mutex m_mutex {};
  };
}
