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

/* stl header */
// #include <format>
#include <memory> // std::unique_ptr

/* fmt header */
#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Weverything"
#endif
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Weffc++"
#endif
#include <fmt/core.h>
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif
#ifdef __clang__
  #pragma clang diagnostic pop
#endif

/* modern.cpp.logger */
#include "Logger.h"

// using fmt::format;

// using vx::logger::Logger::logDebug;

/*namespace std {

  template<typename... T>
  [[nodiscard]] inline auto format( fmt::format_string<T...> fmt, T &&... args ) -> std::string {

    return fmt::format( fmt, args... );
  }
} */

class MyClass {

public:
  explicit MyClass( std::string_view _something )
      : m_something( _something ) {}

  friend std::ostream &operator<<( std::ostream &out, MyClass val ) {

    out << val.m_something;
    return out;
  }

  friend vx::logger::Logger &operator<<( vx::logger::Logger &_debug, MyClass _vec ) {

    _debug.stream() << _vec;
    return _debug;
  }

private:
  std::string_view m_something {};
};

int main() {

  constexpr int aInt = 17;
  constexpr int bInt = 12;
  constexpr double magic = 1.123;
  const std::string test = "blub";
  const char *chr = "test_chr";

  log() << "test" << bInt << test << 1U << magic << chr;
  vx::logger::Logger() << "test2" << aInt;

  const std::vector<std::string> vec { "abc", "def", "ghj" };
  verbose() << vec;

  std::string *str = nullptr;
  debug() << str << false << true << nullptr << 'a';

  const std::unique_ptr<std::string> blub = std::make_unique<std::string>( "blub2" );
  info() << "Pointer:" << blub.get();

  const std::list<std::string> list { "abc", "def", "ghj" };
  warning() << list;

  const std::map<int, std::string> map { { 3, "abc" }, { 1, "def" }, { 2, "ghj" } };
  error() << map;

  const std::unordered_map<int, std::string_view> umap { { 3, "abc" }, { 1, "def" }, { 2, "ghj" } };
  fatal() << umap;

  const std::tuple<int, std::string, std::string_view> tuple { 1, "abc", "def" };
  log() << tuple;

  const std::tuple tuple2 { 1, "abc", "def" };
  log() << tuple2;

  verbose() << std::make_pair( 1, "hello" );

  //  test_debug() << std::make_any<int>(1) << std::make_any<double>(1.234) << std::make_any<std::string>("hello");
  const std::any blubAny = 1;
  debug() << blubAny << std::make_any<std::string>( "hello" ) << std::make_any<std::vector<int>>( { 1, 2, 3, 4 } );

  using namespace std::literals;
  const std::unordered_map<int, std::string_view> testsv { { 2, "ghj"sv }, { 1, "def"sv }, { 3, "abc"sv } };
  (void)testsv;

  //  std::unordered_map testsv2{{2, "ghj"sv}, {1, "def"sv}, {3, "abc"sv}};

  info() << MyClass( "Blub" );

  std::vector<std::any> anyList {};
  anyList.emplace_back( std::make_any<std::string>( "hello" ) );
  anyList.emplace_back( std::make_any<std::vector<int>>( { 1, 2, 3, 4 } ) );
  warning() << anyList;

  const std::set<int> set { 1, 2, 3, 3 };
  error() << set;

  constexpr double magicArr1 = 1.23;
  constexpr double magicArr2 = 1.24;
  constexpr double magicArr3 = 1.25;
  constexpr double magicArr4 = 1.26;

  const std::array<double, 4> arr { magicArr1, magicArr2, magicArr3, magicArr4 };
  fatal() << arr;

  const std::variant<int, double> variant { magicArr3 };
  log() << variant;

  fatal() << fmt::format( "The answer is {}.", 42 );

  constexpr int theAnswerOfEverything = 42;
  constexpr double someDouble = 4.2;

  const std::tuple tupl { theAnswerOfEverything, 'a', someDouble }; // Another C++17 feature: class template argument deduction
  std::apply( []( auto &&...args ) { ( ( debug() << args ), ... ); }, tupl );

  return EXIT_SUCCESS;
}
