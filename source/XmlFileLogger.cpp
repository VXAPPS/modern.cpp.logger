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
#if defined __GNUC__ && __GNUC__ >= 10 || defined _MSC_VER && _MSC_VER >= 1929
  #include <ranges>
#endif

/* magic enum */
#include <magic_enum.hpp>

/* modern.cpp.core */
#include <StringUtils.h>
#include <Timestamp.h>

/* local header */
#include "XmlFileLogger.h"

namespace vx {

  using timestamp::Precision;

  /**
   * @brief Reserved overhead for new xml log entry.
   */
  constexpr int xmlOverhead = 256;

  XmlFileLogger::XmlFileLogger( const std::unordered_map<std::string, std::string> &_configuration )
      : FileLogger( _configuration ) {}

  void XmlFileLogger::log( std::string_view _message,
                           Severity _severity,
                           const std::source_location &_location ) noexcept {

    if ( avoidLogBelow > _severity ) {

      return;
    }

    std::string output {};
    output.reserve( _message.size() + xmlOverhead );
    output.append( "<entry>" );
    output.append( "<timestamp>" );
    output.append( timestamp::iso8601( Precision::MicroSeconds ) );
    output.append( "</timestamp>" );
    if ( std::string( _location.file_name() ) != "unsupported" ) {

      output.append( "<filename>" );
      output.append( _location.file_name() );
      output.append( "</filename>" );
      output.append( "<line>" );
      output.append( std::to_string( _location.line() ) );
      output.append( "</line>" );
      output.append( "<function>" );
      output.append( _location.function_name() );
      output.append( "</function>" );
    }

    std::string severity( magic_enum::enum_name( _severity ) );
    string_utils::toUpper( severity );
    output.append( "<severity>" );
    output.append( severity );
    output.append( "</severity>" );
    output.append( "<message>" );
    output.append( _message );
    output.append( "</message>" );
    output.append( "</entry>\n" );

    FileLogger::log( output );
  }
}
