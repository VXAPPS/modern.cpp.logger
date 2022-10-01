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
#ifndef _WIN32
  #include <cxxabi.h>
#endif

/* stl header */
#include <algorithm>
#include <iostream> // std::streambuf, std::cout
#include <memory>   // std::unique_ptr
#if defined __GNUC__ && __GNUC__ >= 10 || defined _MSC_VER && _MSC_VER >= 1929 || defined __clang__ && __clang_major__ >= 15
  #include <ranges>
#endif
#include <regex>
#include <sstream>

/* magic enum */
#include <magic_enum.hpp>

/* modern.cpp.core */
#include <StringUtils.h>
#include <Timestamp.h>

/* local header */
#include "Logger.h"

namespace vx::logger {

  using timestamp::Precision;

#ifdef _WIN32
  class WindowsBuffer : public std::stringbuf {

  public:
    virtual int sync() override {

      return 0;
    }
  };
#endif

  std::string demangle( const std::string &_name ) {

    std::string result = _name;

#ifndef _WIN32
    /**
     *  0: The demangling operation succeeded.
     * -1: A memory allocation failiure occurred.
     * -2: mangled_name is not a valid name under the C++ ABI mangling rules.
     * -3: One of the arguments is invalid.
     * */
    int status = -4;

    const std::unique_ptr<char, void ( * )( void * )> res {

      abi::__cxa_demangle( _name.c_str(), nullptr, nullptr, &status ),
      std::free
    };

    if ( status == 0 ) {

      result = res.get();
    }
#endif

    // WINDOWS
    result = std::regex_replace( result, std::regex( "class" ), "" );
    result = std::regex_replace( result, std::regex( "struct" ), "" );
    result = std::regex_replace( result, std::regex( "__ptr64" ), "" );
    //    result = std::regex_replace( result, std::regex( "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >" ), "std::string" );
    //    result = std::regex_replace( result, std::regex( "class std::basic_string_view<char,struct std::char_traits<char> >" ), "std::string" );

    // LINUX clang and gcc
    result = std::regex_replace( result, std::regex( "__cxx11::" ), "" );
    //    result = std::regex_replace( result, std::regex( "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >" ), "std::string" );
    //    result = std::regex_replace( result, std::regex( "std::basic_string_view<char, std::char_traits<char> >" ), "std::string_view" );

    // MAC AppleClang
    result = std::regex_replace( result, std::regex( "__1::" ), "" );
    //    result = std::regex_replace( result, std::regex( "std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >" ), "std::string" );
    //    result = std::regex_replace( result, std::regex( "std::__1::basic_string_view<char, std::__1::char_traits<char> >" ), "std::string_view" );

    // All, after general cleanup
    result = std::regex_replace( result, std::regex( "std::basic_string<char, std::char_traits<char>, std::allocator<char> >" ), "std::string" );
    result = std::regex_replace( result, std::regex( "std::basic_string_view<char, std::char_traits<char> >" ), "std::string_view" );

    // Will result to const char**??? Maybe, have an look at regex documentation...
    //    result = std::regex_replace( result, std::regex( "char const *" ), "const char *" );
    //    result = std::regex_replace( result, std::regex( "char const*" ), "const char *" );

    // Remove space before closing bracket - overall valid
    result = std::regex_replace( result, std::regex( " >" ), ">" );
    result = string_utils::simplified( result );

    return result;
  }

  Logger::Logger( Severity _severity,
                  const std::source_location &_location )
      : m_severity( _severity ),
        m_location( _location ),
#ifdef _WIN32
        m_stream( new WindowsBuffer() ) {
#else
        m_stream( {} ) {
#endif

    if ( _severity >= Severity::Error ) {

      m_stream.rdbuf( std::cerr.rdbuf() );
    }
    else {

      m_stream.rdbuf( std::cout.rdbuf() );
    }
    m_stream << timestamp() << ' ';
    m_stream << severity( m_severity ) << ' ';
    if ( std::string filename = _location.file_name(); filename != "unsupported" ) {

#ifdef _WIN32
      constexpr char delimiter = '\\';
#else
      constexpr char delimiter = '/';
#endif
      if ( m_locationPath == Path::Filename && filename.find_last_of( delimiter ) != std::string::npos ) {

        const std::size_t pos = filename.find_last_of( delimiter );
        filename = filename.substr( pos + 1, filename.size() - ( pos + 1 ) );
      }
      else if ( m_locationPath == Path::Relative && filename.find_last_of( delimiter ) != std::string::npos ) {

        /* TODO: Find a better solution for the real project_source_dir, instead of two folders back if available */
        std::size_t pos = filename.find_last_of( delimiter );
        const std::size_t secondPos = filename.find_last_of( delimiter, pos - 1 );
        if ( secondPos != std::string::npos ) {

          pos = secondPos;
          const std::size_t thirdPos = filename.find_last_of( delimiter, pos - 1 );
          if ( thirdPos != std::string::npos ) {

            pos = thirdPos;
          }
        }
        filename = filename.substr( pos + 1, filename.size() - ( pos + 1 ) );
      }
      m_stream << filename << ':' << _location.line() << ' ' << _location.function_name() << ' ';
    }
  }

  Logger::~Logger() {

    m_stream << std::endl;
  }

  void Logger::printString( std::string_view _input ) {

    m_autoQuotes ? m_stream << std::quoted( _input ) : m_stream << _input;
  }

  std::string Logger::timestamp() const {

    return timestamp::iso8601( Precision::MicroSeconds );
  }

  std::string Logger::severity( Severity _severity ) const {

    std::string result {};
    std::string severity( magic_enum::enum_name( _severity ) );
    string_utils::toUpper( severity );
    switch ( _severity ) {

      case Severity::Verbose:
        result += "\x1b[37;1m[" + severity + "]\x1b[0m";
        break;
      case Severity::Debug:
        result += "  \x1b[34;1m[" + severity + "]\x1b[0m";
        break;
      case Severity::Info:
        result += "   \x1b[32;1m[" + severity + "]\x1b[0m";
        break;
      case Severity::Warning:
        result += "\x1b[33;1m[" + severity + "]\x1b[0m";
        break;
      case Severity::Error:
        result += "  \x1b[31;1m[" + severity + "]\x1b[0m";
        break;
      case Severity::Fatal:
        result += "  \x1b[41;1m[" + severity + "]\x1b[0m";
        break;
    }
    return result;
  }
}
