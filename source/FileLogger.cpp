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
#include <mutex>

/* magic enum */
#include <magic_enum.hpp>

/* local header */
#include "FileLogger.h"

namespace vx {

  /**
   * @brief Reserved overhead for new log entry.
   */
  constexpr int fileOverhead = 64;

  /**
   * @brief Default reopen interval.
   */
  constexpr int reopenInterval = 300;

  FileLogger::FileLogger( const std::unordered_map<std::string, std::string> &_configuration )
    : Logger( _configuration ) {

    /* grab the file name */
    const auto name = _configuration.find( "filename" );
    if ( name == _configuration.end() ) {

      throw std::invalid_argument( "No output file provided to file logger." );
    }
    m_filename = name->second;

    /* if we specify an interval */
    m_reopenInterval = std::chrono::seconds( reopenInterval );
    const auto interval = _configuration.find( "reopen_interval" );
    if ( interval != _configuration.end() ) {

      try {

        m_reopenInterval = std::chrono::seconds( std::stoul( interval->second ) );
      }
      catch ( [[maybe_unused]] const std::invalid_argument &_exception ) {

        throw std::invalid_argument( interval->second + " is not a valid reopen interval." );
      }
      catch ( [[maybe_unused]] const std::out_of_range &_exception ) {

        throw std::out_of_range( interval->second + " is out of range reopen interval." );
      }
      catch ( [[maybe_unused]] const std::exception &_exception ) {

        /* nothing to do */
      }
    }

    m_file.exceptions( std::ofstream::failbit | std::ofstream::badbit );

    /* open the file */
    reopen();
  }

  FileLogger::~FileLogger() noexcept {

    try {

      if ( m_file.is_open() ) {

        m_file.close();
      }
    }
    catch ( [[maybe_unused]] const std::exception &_exception ) {

      /* nothing to do, file cannot be closed. */
    }
  }

  void FileLogger::log( std::string_view _message,
                        const Severity _severity,
                        const nostd::source_location &_location ) noexcept {

    if ( avoidLogBelow > _severity ) {

      return;
    }

    std::string output {};
    output.reserve( _message.size() + fileOverhead );
    output.append( timestamp() );

    std::string severity( magic_enum::enum_name( _severity ) );
    std::transform( std::begin( severity ), std::end( severity ), std::begin( severity ), []( auto c ) { return ::toupper( c ); } );
    output.append( " [" + severity + "] " );
    if ( std::string( _location.file_name() ) != "unsupported" ) {

      output.append( _location.file_name() );
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

  void FileLogger::log( std::string_view _message ) noexcept {

    std::unique_lock<std::shared_mutex> lock( m_mutex );

    m_file << _message;
    m_file.flush();

    lock.unlock();

    reopen();
  }

  void FileLogger::reopen() noexcept {

    std::shared_lock<std::shared_mutex> lock( m_mutex );

    /* check if it should be closed and reopened */
    const auto now = std::chrono::system_clock::now();
    if ( now - m_lastReopen > m_reopenInterval ) {

      m_lastReopen = now;

      try {

        if ( m_file.is_open() ) {

          m_file.close();
        }
        m_file.open( m_filename, std::ofstream::out | std::ofstream::app );
      }
      catch ( [[maybe_unused]] const std::exception &_exception ) {

        /* nothing to do, file is not open or cannot be closed. */
      }
      m_lastReopen = std::chrono::system_clock::now();
    }
  }
}
