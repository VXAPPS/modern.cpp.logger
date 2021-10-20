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

/* stl header */
#include <stdexcept>

/* local header */
#include "FileLogger.h"
#include "LoggerFactory.h"
#include "StdLogger.h"
#include "XmlFileLogger.h"

namespace vx {

  LoggerFactory::LoggerFactory() noexcept {

    m_creators.try_emplace( "", []( const std::unordered_map<std::string, std::string> &_configuration ) { return std::make_unique<Logger>( _configuration ); } );
    m_creators.try_emplace( "std", []( const std::unordered_map<std::string, std::string> &_configuration ) -> std::unique_ptr<Logger> { return std::make_unique<StdLogger>( _configuration ); } );
    m_creators.try_emplace( "file", []( const std::unordered_map<std::string, std::string> &_configuration ) -> std::unique_ptr<Logger> { return std::make_unique<FileLogger>( _configuration ); } );
    m_creators.try_emplace( "xml", []( const std::unordered_map<std::string, std::string> &_configuration ) -> std::unique_ptr<Logger> { return std::make_unique<XmlFileLogger>( _configuration ); } );
  }

  std::unique_ptr<Logger> LoggerFactory::produce( const std::unordered_map<std::string, std::string> &_configuration ) const {

    /* grab the type */
    auto type = _configuration.find( "type" );
    if ( type == _configuration.end() ) {

      throw std::invalid_argument( "Logging factory configuration requires a type of logger" );
    }

    /* grab the logger */
    if ( auto found = m_creators.find( type->second ); found != m_creators.end() ) {

      return found->second( _configuration );
    }

    /* couldn't get a logger */
    throw std::invalid_argument( "Couldn't produce logger of type: " + type->second );
  }
}
