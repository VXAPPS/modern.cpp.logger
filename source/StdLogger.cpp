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
#include <algorithm>
#include <iostream>
#include <sstream>

/* magic enum */
#include <magic_enum.hpp>

/* local header */
#include "StdLogger.h"

namespace vx {

  /**
   * @brief Reserved overhead for new log entry.
   */
  constexpr int overhead = 64;

  StdLogger::StdLogger( const std::unordered_map<std::string, std::string> &_configuration ) noexcept
    : Logger( _configuration )
    , m_useColor( _configuration.find( "color" ) != std::end( _configuration ) )
    , m_useStdErr( _configuration.find( "stderr" ) != std::end( _configuration ) ) {}

  void StdLogger::log( std::string_view _message,
                       const Severity _severity,
                       const nostd::source_location &_location ) noexcept {

    if ( avoidLogBelow > _severity ) {

      return;
    }

    std::string output;
    output.reserve( _message.size() + overhead );
    output.append( timestamp() );

    std::string severity( magic_enum::enum_name( _severity ) );
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto c ) { return ::toupper( c ); } );
    if ( m_useColor ) {

      switch ( _severity ) {

        case Severity::Verbose:
          output.append( " \x1b[37;1m[" + severity + "]\x1b[0m " );
          break;
        case Severity::Debug:
          output.append( " \x1b[34;1m[" + severity + "]\x1b[0m " );
          break;
        case Severity::Info:
          output.append( " \x1b[32;1m[" + severity + "]\x1b[0m " );
          break;
        case Severity::Warning:
          output.append( " \x1b[33;1m[" + severity + "]\x1b[0m " );
          break;
        case Severity::Error:
          output.append( " \x1b[31;1m[" + severity + "]\x1b[0m " );
          break;
        case Severity::Fatal:
          output.append( " \x1b[41;1m[" + severity + "]\x1b[0m " );
          break;
      }
    }
    else {

      output.append( " [" + severity + "] " );
    }

    if ( std::string filename = _location.file_name(); filename != "unsupported" ) {

      if ( filename.find_last_of( '/' ) != std::string::npos ) {

        std::size_t pos = filename.find_last_of( '/' );
        filename = filename.substr( pos + 1, filename.size() - ( pos + 1 ) );
      }
      output.append( filename );
      output.push_back( ':' );
      output.append( std::to_string( _location.line() ) );
      output.push_back( ' ' );
      output.append( _location.function_name() );
      output.push_back( ' ' );
    }
    output.append( _message );
    output.push_back( '\n' );

    log( output );
  }

  void StdLogger::log( std::string_view _message ) noexcept {

    /* cout is thread safe, to avoid multiple threads interleaving on one line */
    /* though, we make sure to only call the << operator once on std::cout */
    /* otherwise the << operators from different threads could interleave */
    /* obviously we dont care if flushes interleave */
    std::cout << _message;
    std::cout.flush();
  }
}
