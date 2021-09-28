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
#include <memory>
#include <string>
#include <unordered_map>

/* modern.cpp.core */
#include <Singleton.h>

/* local header */
#include "Logger.h"

/**
 * @brief vx (VX APPS) namespace.
 */
namespace vx {

  /**
   * @brief The LoggerFactory class.
   * @author Florian Becker <fb\@vxapps.com> (VX APPS)
   */
  class LoggerFactory : public Singleton<LoggerFactory> {

  public:
    /**
     * @brief Default constructor for LoggerFactory.
     */
    explicit LoggerFactory();

    /**
     * @brief Create all siblings of different log possibilities.
     * @param _configuration   Configuration for logger.
     * @return Logger class with specific config.
     */
    Logger *produce( const std::unordered_map<std::string, std::string> &_configuration ) const;

  private:
    /**
     * @brief Created loggers.
     */
    std::unordered_map<std::string, Logger *( * )( const std::unordered_map<std::string, std::string> & )> m_creators;
  };
}

/**
 * @brief Create instance with default configuration for logger.
 * @param _configuration   Configuration for logger.
 */
inline vx::Logger &instance( const std::unordered_map<std::string, std::string> &_configuration = { { "type", "std" }, { "color", "" } } ) {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
  static std::unique_ptr<vx::Logger> singleton( vx::LoggerFactory::instance().produce( _configuration ) );
  return *singleton;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

/**
 * @brief Change the configuration at runtime.
 * @param _configuration   Logger configuration.
 */
inline void ConfigureLogger( const std::unordered_map<std::string, std::string> &_configuration ) {

  instance( _configuration );
}

/**
 * @brief Direct function for logging.
 * @param _message   Message to log.
 * @param _severity   Severity level for message to log.
 * @param _location   Source location information.
 */
inline void Log( const std::string &_message,
                 const Severity _severity,
                 const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, _severity, _location );
}

/**
 * @brief Direct function for logging.
 * @param _message   Message to log.
 */
inline void Log( const std::string &_message ) {

  instance().log( _message );
}

/**
 * @brief Direct function for logging with verbose serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogVerbose( const std::string &_message,
                        const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Verbose, _location );
}

/**
 * @brief Direct function for logging with debug serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogDebug( const std::string &_message,
                      const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Debug, _location );
}

/**
 * @brief Direct function for logging with info serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogInfo( const std::string &_message,
                     const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Info, _location );
}

/**
 * @brief Direct function for logging with warning serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogWarning( const std::string &_message,
                        const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Warning, _location );
}

/**
 * @brief Direct function for logging with error serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogError( const std::string &_message,
                      const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Error, _location );
}

/**
 * @brief Direct function for logging with fatal error serivity.
 * @param _message   Message to log.
 * @param _location   Source location information.
 */
inline void LogFatal( const std::string &_message,
                      const nostd::source_location &_location = nostd::source_location::current() ) {

  instance().log( _message, Severity::Fatal, _location );
}
