/*
 * Copyright (c) 2021 Florian Becker <fb@vxapps.com> (VX APPS).
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
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

/* modern.cpp.logger */
#include <LoggerFactory.h>

/**
 * @brief Filename of temporary log file.
 */
constexpr auto filename = "example.log";

/**
 * @brief Log message itself.
 */
constexpr auto logMessage = "This is a log message";

/**
 * @brief Function for log some messages.
 */
static void logSomeMessages( vx::Logger *_logger ) {

  _logger->log( logMessage, vx::Severity::Fatal );
  _logger->log( logMessage, vx::Severity::Error );
  _logger->log( logMessage, vx::Severity::Warning );
  _logger->log( logMessage, vx::Severity::Info );
  _logger->log( logMessage, vx::Severity::Debug );
  _logger->log( logMessage, vx::Severity::Verbose );
}

int main() {

  /* configure logging, if you dont it defaults to standard out logging with colors */
  std::cout << "Log to std::cout" << std::endl;
  const std::unique_ptr<vx::Logger> stdLogger( vx::LoggerFactory::instance().produce( { { "type", "std" }, { "color", "true" } } ) );

  /* Log some messages */
  logSomeMessages( stdLogger.get() );

  /* create tmp file */
  std::error_code errorCode {};
  std::filesystem::path tmpPath = std::filesystem::temp_directory_path( errorCode );
  if ( errorCode ) {

    std::cout << "Error getting temp_directory_path: " << errorCode.message() << " Code: " << errorCode.value() << std::endl;
    return EXIT_FAILURE;
  }
  tmpPath /= filename;
  std::string tmpFile = tmpPath.string();
  std::cout << "Create tmp file: " << tmpFile << std::endl;

  /* configure logging, if you dont it defaults to standard out logging with colors */
  std::cout << "Log to file" << std::endl;
  const std::unique_ptr<vx::Logger> fileLogger( vx::LoggerFactory::instance().produce( { { "type", "file" }, { "filename", tmpFile }, { "reopen_interval", "1" } } ) );

  /* Log some messages */
  logSomeMessages( fileLogger.get() );

  /* remove tmp file */
  if ( !std::filesystem::remove( tmpFile ) ) {

    std::cout << "Tmp file cannot be removed: " << tmpFile << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Tmp file was removed: " << tmpFile << std::endl;
  return EXIT_SUCCESS;
}
