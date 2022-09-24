/*
 * Copyright (c) 2022 Florian Becker <fb@vxapps.com> (VX APPS).
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

/* c header */
#include <cxxabi.h>

/* stl header */
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream> // std::streambuf, std::cout
#include <memory> // std::unique_ptr
#if defined __GNUC__ && __GNUC__ >= 10 || defined _MSC_VER && _MSC_VER >= 1929
  #include <ranges>
#endif
#include <regex>

/* magic enum */
#include <magic_enum.hpp>

/* local header */
#include "Logger.h"

namespace vx::logger {

  std::string demangle( const std::string &_name ) {

    /**
     *  0: The demangling operation succeeded.
     * -1: A memory allocation failiure occurred.
     * -2: mangled_name is not a valid name under the C++ ABI mangling rules.
     * -3: One of the arguments is invalid.
     * */
    int status = -4;

    const std::unique_ptr<char, void( * )( void * )> res {

      abi::__cxa_demangle( _name.c_str(), nullptr, nullptr, &status ),
      std::free
    };

    std::string result = _name;
    if ( status == 0 ) {

      result = res.get();
    }

    // LINUX clang and gcc
    result = std::regex_replace( result, std::regex( "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >" ), "std::string" );
    result = std::regex_replace( result, std::regex( "std::basic_string_view<char, std::char_traits<char> >" ), "std::string_view" );

//    result = std::regex_replace( result, std::regex( "char const*" ), "const char *" );

    // Remove space before closing bracket - overall valid
    result = std::regex_replace( result, std::regex( " >" ), ">" );

    return result;
  }

  Logger::Logger( Severity _severity,
                  const std::source_location &_location )
    : m_severity( _severity ),
      m_location( _location ),
      m_stream( {} ) {

    if ( _severity >= Severity::Error ) {

      m_stream.rdbuf( std::cerr.rdbuf() );
    }
    else {

      m_stream.rdbuf( std::cout.rdbuf() );
    }
    printTimestamp();
    printSeverity( m_severity );
    if ( std::string filename = _location.file_name(); filename != "unsupported" ) {

      if ( m_sourceLocation == SourceLocation::FilenameOnly && filename.find_last_of( '/' ) != std::string::npos ) {

        const std::size_t pos = filename.find_last_of( '/' );
        filename = filename.substr( pos + 1, filename.size() - ( pos + 1 ) );
      }
      m_stream << filename << ':' << _location.line() << ' ' << _location.function_name() << ' ';
    }
  }

  Logger::~Logger() {

    m_stream << std::endl;
  }

  void Logger::printTimestamp() {

    /* get a precise timestamp as a string */
    struct std::tm currentLocalTime {};
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t( now );
    const auto nowMs = std::chrono::duration_cast<std::chrono::microseconds>( now.time_since_epoch() ) % 1000000;
    constexpr int mirosecondsPrecision = 6;

#ifdef _WIN32
    localtime_s( &currentLocalTime, &nowAsTimeT );
#else
    localtime_r( &nowAsTimeT, &currentLocalTime );
#endif

    std::ostringstream nowSs;
    nowSs
        << std::put_time( &currentLocalTime, "%Y-%m-%dT%T" )
        << '.' << std::setfill( '0' ) << std::setw( mirosecondsPrecision ) << nowMs.count()
        << std::put_time( &currentLocalTime, "%z" );
    std::string result = nowSs.str();
    /* somewhat special - maybe see systemtimeformatter */
    result.replace( result.end() - 2, result.end() - 2, ":" );
    m_stream << result << ' ';
  }

  void Logger::printSeverity( Severity _severity ) {

    std::string severity( magic_enum::enum_name( _severity ) );
    /* Visual Studio 2017 does not handle toupper in std namespace */
#if defined _MSC_VER && _MSC_VER < 1920
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto chr ) { return ::toupper( chr ); } );
#else
#if defined __GNUC__ && __GNUC__ >= 10 || defined _MSC_VER && _MSC_VER >= 1929
    std::ranges::transform( severity, std::begin( severity ), []( auto chr ) { return std::toupper( chr ); } );
#else
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto chr ) { return std::toupper( chr ); } );
#endif
#endif

    switch ( _severity ) {

      case Severity::Verbose:
        m_stream << "\x1b[37;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Debug:
        m_stream << "  \x1b[34;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Info:
        m_stream << "   \x1b[32;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Warning:
        m_stream << "\x1b[33;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Error:
        m_stream << "  \x1b[31;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Fatal:
        m_stream << "  \x1b[41;1m[" << severity << "]\x1b[0m";
        break;
    }
    m_stream << ' ';
  }

  void Logger::printString( std::string_view _input ) {

    m_autoQuotes ? m_stream << std::quoted( _input ) : m_stream << _input;
  }

  Logger &Logger::operator<<( Severity _severity ) {

    std::string severity( magic_enum::enum_name( _severity ) );
    /* Visual Studio 2017 does not handle toupper in std namespace */
#if defined _MSC_VER && _MSC_VER < 1920
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto chr ) { return ::toupper( chr ); } );
#else
#if defined __GNUC__ && __GNUC__ >= 10 || defined _MSC_VER && _MSC_VER >= 1929
    std::ranges::transform( severity, std::begin( severity ), []( auto chr ) { return std::toupper( chr ); } );
#else
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto chr ) { return std::toupper( chr ); } );
#endif
#endif

    switch ( _severity ) {

      case Severity::Verbose:
        m_stream << "\x1b[37;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Debug:
        m_stream << "\x1b[34;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Info:
        m_stream << "\x1b[32;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Warning:
        m_stream << "\x1b[33;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Error:
        m_stream << "\x1b[31;1m[" << severity << "]\x1b[0m";
        break;
      case Severity::Fatal:
        m_stream << "\x1b[41;1m[" << severity << "]\x1b[0m";
        break;
    }
    return maybeSpace();
  }
}
