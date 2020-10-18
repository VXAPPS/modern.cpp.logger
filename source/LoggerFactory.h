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
#include <string>
#include <unordered_map>

/* modern.cpp.core */
#include <Singleton.h>

/* local header */
#include "Logger.h"

/**
 * @brief vx (VX Apps) namespace.
 */
namespace vx {

  /* a factory that can create loggers (that derive from 'logger') via function pointers */
  /* this way you could make your own logger that sends log messages to who knows where */
  class LoggerFactory : public Singleton<LoggerFactory> {

  public:
    explicit LoggerFactory();

    Logger *produce( const std::unordered_map<std::string, std::string> &_config ) const;

  private:
    std::unordered_map<std::string, Logger *( * )( const std::unordered_map<std::string, std::string> & )> m_creators;
  };

  /* get at the singleton */
  inline Logger &instance( const std::unordered_map<std::string, std::string> &_config = { { "type", "std" }, { "color", "" } } ) {

    static std::unique_ptr<Logger> singleton( LoggerFactory::instance().produce( _config ) );
    return *singleton;
  }

  /* configure the singleton (once only) */
  inline void LogConfigure( const std::unordered_map<std::string, std::string> &_config ) {

    instance( _config );
  }

  /* statically log manually without the macros below */
  inline void Log( const std::string &_message, const Severity _level ) {

    instance().log( _message, _level );
  }

  /* statically log manually without a level or maybe with a custom one */
  inline void Log( const std::string &_message ) {

    instance().log( _message );
  }

  /* these standout when reading code */
  inline void LogVerbose( const std::string &_message ) {

    instance().log( _message, Severity::Verbose );
  };

  inline void LogDebug( const std::string &_message ) {

    instance().log( _message, Severity::Debug );
  };

  inline void LogInfo( const std::string &_message ) {

    instance().log( _message, Severity::Info );
  };

  inline void LogWarning( const std::string &_message ) {

    instance().log( _message, Severity::Warning );
  };

  inline void LogError( const std::string &_message ) {

    instance().log( _message, Severity::Error );
  };

  inline void LogFatal( const std::string &_message ) {

    instance().log( _message, Severity::Fatal );
  };
}
