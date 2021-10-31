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

/* cppunit header */
#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Weverything"
#endif
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Weffc++"
#endif
#include <cppunit/TestCase.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif
#ifdef __clang__
  #pragma clang diagnostic pop
#endif

/* stl header */
#include <thread>

/* modern.cpp.logger header */
#include <LoggerFactory.h>

/**
 * @brief Count of log messages per thread.
 */
constexpr std::size_t logMessageCount = 10000;

/**
 * @brief Log message itself.
 */
constexpr auto logMessage = "This is a log message";

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
  #pragma clang diagnostic ignored "-Wweak-vtables"
#endif
namespace vx {

  class ThreadsStdLogger : public CppUnit::TestCase {

    CPPUNIT_TEST_SUITE_REGISTRATION( ThreadsStdLogger );
    CPPUNIT_TEST_SUITE( ThreadsStdLogger );
    CPPUNIT_TEST( stdLogger );
    CPPUNIT_TEST_SUITE_END();

  public:
    explicit ThreadsStdLogger( const std::string &_name = {} ) noexcept : CppUnit::TestCase( _name ) {}

    void setUp() noexcept final { /* Setup things here. */ }

    virtual void stdLogger() noexcept {

      /* configure logging, if you dont do, it defaults to standard out logging with colors */
      /* ConfigureLogger( { { "type", "std" }, { "color", "true" } } ); */

      unsigned int hardwareThreadCount = std::max<unsigned int>( 1, std::thread::hardware_concurrency() );

      std::vector<std::thread> threads {};
      threads.reserve( hardwareThreadCount );
      for ( unsigned int n = 0; n < hardwareThreadCount; ++n ) {

        threads.emplace_back( std::thread( [&hardwareThreadCount] {

          std::ostringstream s;
          s << logMessage;

          std::string message = s.str();
          for ( std::size_t i  = 0; i < logMessageCount / hardwareThreadCount; ++i ) {

            LogFatal( message );
            LogError( message );
            LogWarning( message );
            LogInfo( message );
            LogDebug( message );
            LogVerbose( message );
          }
        } ) );
      }
      for ( auto &thread : threads ) {

        thread.join();
      }
      threads.clear();

      CPPUNIT_ASSERT( true );
    }

    void tearDown() noexcept final { /* Clean up things here. */ }
  };
}
#ifdef __clang__
  #pragma clang diagnostic pop
#endif

int main() {

  CppUnit::TextUi::TestRunner runner;
  runner.addTest( vx::ThreadsStdLogger::suite() );
  bool wasSuccessful = runner.run();

  std::ofstream xmlFileOut( "../ThreadsStdLogger.xml" );
  CppUnit::XmlOutputter xmlOut( &runner.result(), xmlFileOut );
  xmlOut.write();
  try {

    xmlFileOut.close();
  }
  catch ( const std::exception &_exception ) {

    /* Do not throw exception here */
    std::cout << _exception.what() << std::endl;
  }

  return wasSuccessful ? 0 : 1;
}
