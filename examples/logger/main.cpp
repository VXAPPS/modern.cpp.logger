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
#include <thread>
#include <vector>

/* modern.cpp.logger */
//#include <FileLogger.h>
#include <LoggerFactory.h>
//#include <Logger.h>
//#include <StdLogger.h>
//#include <XmlFileLogger.h>

using namespace vx;

constexpr auto logMessage = "This is a log message";

//static void work() {

//  std::ostringstream s;
//  s << logMessage;
//  std::string message = s.str();
//  for ( size_t i  = 0; i < 10000; ++i ) {

//    vx::LogFatal( message );
//    vx::LogError( message );
//    vx::LogWarning( message );
//    vx::LogInfo( message );
//    vx::LogDebug( message );
//    vx::LogVerbose( message );
//  }
//}

int main() {

  /* configure logging, if you dont it defaults to standard out logging with colors */
//  vx::configureLogger({ {"type", "file"}, {"filename", "test2.log"}, {"reopen_interval", "1"} });

  /* start up some threads */
/*  std::vector<std::shared_ptr<std::thread>> threads( std::thread::hardware_concurrency() );
  for ( auto &thread : threads ) {

    thread.reset( new std::thread( work ) );
  } */

  /* wait for finish */
/*  for ( const auto &thread : threads ) {

    thread->join();
  } */

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

  return EXIT_SUCCESS;
}
