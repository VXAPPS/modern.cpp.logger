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
#include <filesystem>
#include <string>
#include <thread>
#include <vector>

/* modern.cpp.logger */
//#include <FileLogger.h>
#include <LoggerFactory.h>
//#include <Logger.h>
//#include <StdLogger.h>
//#include <XmlFileLogger.h>
#include <iostream>

/**
 * @brief Filename of temporary log file.
 */
constexpr auto filename = "thread-test.log";

/**
 * @brief Count of log messages per thread.
 */
constexpr std::size_t logMessageCount = 10000;

/**
 * @brief Log message itself.
 */
constexpr auto logMessage = "This is a log message";

/**
 * @brief Function per thread.
 */
static void work() {

  std::ostringstream s;
  s << logMessage;

  std::string message = s.str();
  for ( std::size_t i  = 0; i < logMessageCount; ++i ) {

    LogFatal( message );
    LogError( message );
    LogWarning( message );
    LogInfo( message );
    LogDebug( message );
    LogVerbose( message );
  }
}

int main() {

  std::cout << std::filesystem::temp_directory_path() << std::endl;
  std::filesystem::path tmpPath = std::filesystem::temp_directory_path();
  tmpPath /= filename;
  std::string tmpFile = tmpPath.string();
  std::cout << "Create tmp file: " << tmpFile << std::endl;

  /* configure logging, if you dont it defaults to standard out logging with colors */
  ConfigureLogger( { { "type", "file" }, { "filename", tmpFile }, { "reopen_interval", "1" } } );

  /* start up some threads */
  unsigned int hardwareThreadCount = std::max<unsigned int>( 1, std::thread::hardware_concurrency() );
  std::cout << "Using threads: " << hardwareThreadCount << std::endl;

  std::vector<std::thread> threads {};
  threads.reserve( hardwareThreadCount );
  for ( unsigned int i = 0; i < hardwareThreadCount; ++i ) {

    threads.emplace_back( std::thread( work ) );
  }
  for ( auto &thread : threads ) {

    thread.join();
  }
  threads.clear();

  // = { { "type", "file" }, { "filename", "test2.log" }, { "reopen_interval", "1" } };

  /* Log to /dev/null */
  /* std::unordered_map<std::string, std::string> configuration;
  Logger logger( configuration );
  logger.log( logMessage, Severity::Fatal ); */

  /* Log to stdout */
//  std::unordered_map<std::string, std::string> configuration = { { "color", "" } };
//  StdLogger logger( configuration );
//  logger.log( logMessage, Severity::Fatal );

  /* Log to file */
  /* std::unordered_map<std::string, std::string> configuration = { { "color", "" }, {"filename", "logger.log"} };
  FileLogger logger( configuration );
  logger.log( logMessage, Severity::Fatal ); */

  /* Log to file */
//  std::unordered_map<std::string, std::string> configuration = { { "color", "" }, {"filename", "logger.xml"} };
//  XmlFileLogger logger( configuration );
//  logger.log( logMessage, Severity::Fatal );

  /* Log with logging factory */
  LogFatal( logMessage );

  bool removed = std::filesystem::remove( tmpFile );
  if ( !removed ) {

    std::cout << "Tmp file cannot be removed: " << tmpFile << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Tmp file was removed: " << tmpFile << std::endl;

  return EXIT_SUCCESS;
}
