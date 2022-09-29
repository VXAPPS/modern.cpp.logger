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
#include <format>
#include <functional>
#include <iostream>
#include <memory> // std::unique_ptr

/* magic_enum */
#include <magic_enum.hpp>

/* modern.cpp.logger */
#include "Logger.h"

//using debug = vx::logger::Logger( vx::logger::Severity::Debug );

using vx::logger::Logger;
//using myLogger = vx::logger::Logger;

/*namespace vx::logger {

  std::function<( Logger & ( Logger ) )> f_add_display = &Logger::debug(); //::debug();
}*/

//using myInt = int;

using namespace magic_enum::ostream_operators;

class MyClass {

public:
  explicit MyClass( std::string_view _something )
      : m_something( _something ) {}

  void test() { std::cout << m_something << std::endl; }

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

/* template <int... I>
struct indices {
  static constexpr int N = sizeof...( I );
};

typedef indices<0, 2, 3> IS;

template <class Tuple, int... Is>
static void magic( Tuple &&args, indices<Is...> ) {
  //    B<IS::N>::zoo(std::get<Is>(std::forward<Tuple>(args))...);
  for ( std::size_t x = 0; x < std::tuple_size<Tuple>(); x++ ) {
    std::cout << std::get<0>( args ) << std::endl;
    std::cout << std::get<1>( args ) << std::endl;
    std::cout << std::get<2>( args ) << std::endl;
  }
}

template <typename... Args>
static void foo( Args... args ) {
  //    static_assert(sizeof...(args) == M, "");
  magic( std::forward_as_tuple( args... ), IS {} );
} */

/* namespace cxx20 {
  enum class long_long_long_name { x,
                                   y };

  static void using_enum_demo() {
    std::cout << "C++20 `using enum`: __cpp_using_enum == ";
    switch ( auto rnd = [] { return long_long_long_name::x; }; rnd() ) {
#if defined( __cpp_using_enum )
      using enum long_long_long_name;
      case x:
        std::cout << __cpp_using_enum << "; x\n";
        break;
      case y:
        std::cout << __cpp_using_enum << "; y\n";
        break;
#else
      case long_long_long_name::x:
        std::cout << "?; x\n";
        break;
      case long_long_long_name::y:
        std::cout << "?; y\n";
        break;
#endif
    }
  }
} */

int main() {

  //  cxx20::using_enum_demo();
  //  std::function<void( Foo3 * )> f2 = &Foo3::doSomething;
  //  std::function<int( Foo3 *, int )> f3 = &Foo3::doSomething2;
  //  auto f = Foo {};

  std::function<void( MyClass * )> f4 = &MyClass::test;

  auto legger = std::mem_fn( &Logger::logger );
  legger( Logger() ) << 1 << 2 << "3";
  //  std::function<Logger &( void )> g = []() { return Logger().loggerWithoutDefine(); };
  //  log2() << 1 << 2 << 3 << 4;

  //  std::function<void( Logger * )> lug = &Logger::printTimestamp;
  //  std::function<Logger &( Logger * )> deb = &Logger::loggerWithoutDefine;
  //  deb( Logger() ) << 1 << 2 << "3";
  //  qwe = &vx::logger::Logger::debug();

  //  auto greet = std::mem_fn( &Foo::display_greeting );
  //  greet( f );

  //  auto print_num = std::mem_fn( &Foo::display_number );
  //  print_num( f, 42 );

  //  vx::logger::Logger leg;
  //  auto greet2 = std::mem_fn( &Foo3::doSomething() );
  //  using func = std::function<myLogger &( myLogger & MyLogger )>;
  //  func f_add_display = &myLogger::printTimestamp();

  // Store member function binding
  //  MyClass myClass;
  //  auto callable = std::mem_fn( &MyClass::test() );

  // Call with late supplied 'this'
  //  MyClass myInst;
  //  callable( &leg );

  constexpr int aInt = 17;
  constexpr int bInt = 12;
  constexpr double magic = 1.123;
  const std::string test = "blub";
  const char *chr = "test_chr";

  logInfo() << "test" << bInt << test << 1U << magic << chr;
  vx::logger::Logger() << "test2" << aInt;

  const std::vector<std::string> vec { "abc", "def", "ghj" };
  logVerbose() << vec;

  std::string *str = nullptr;
  logDebug() << str << false << true << nullptr << 'a';

  const std::unique_ptr<std::string> blub = std::make_unique<std::string>( "blub2" );
  logInfo() << "Pointer:" << blub.get();

  const std::list<std::string> list { "abc", "def", "ghj" };
  logWarning() << list;

  const std::map<int, std::string> map { { 3, "abc" }, { 1, "def" }, { 2, "ghj" } };
  logError() << map;

  const std::unordered_map<int, std::string_view> umap { { 3, "abc" }, { 1, "def" }, { 2, "ghj" } };
  logFatal() << umap;

  const std::tuple<int, std::string, std::string_view> tuple { 1, "abc", "def" };
  logInfo() << tuple;

  const std::tuple tuple2 { 1, "abc", "def" };
  logInfo() << tuple2;

  logVerbose() << std::make_pair( 1, "hello" );

  //  test_debug() << std::make_any<int>(1) << std::make_any<double>(1.234) << std::make_any<std::string>("hello");
  const std::any blubAny = 1;
  const std::any helo = std::make_any<std::string>( "hello" );
  logDebug() << blubAny << helo << std::make_any<std::vector<int>>( { 1, 2, 3, 4 } );

  using namespace std::literals;
  const std::unordered_map<int, std::string_view> testsv { { 2, "ghj"sv }, { 1, "def"sv }, { 3, "abc"sv } };
  (void)testsv;

  //  std::unordered_map testsv2{{2, "ghj"sv}, {1, "def"sv}, {3, "abc"sv}};

  logInfo() << MyClass( "Blub" );

  std::vector<std::any> anyList {};
  anyList.emplace_back( std::make_any<std::string>( "hello" ) );
  anyList.emplace_back( std::make_any<std::vector<int>>( { 1, 2, 3, 4 } ) );
  logWarning() << anyList;

  const std::set<int> set { 1, 2, 3, 3 };
  logError() << set;

  constexpr double magicArr1 = 1.23;
  constexpr double magicArr2 = 1.24;
  constexpr double magicArr3 = 1.25;
  constexpr double magicArr4 = 1.26;

  const std::array<double, 4> arr { magicArr1, magicArr2, magicArr3, magicArr4 };
  logFatal() << arr;

  const std::variant<int, double> variant { magicArr3 };
  logInfo() << variant;

  constexpr int theAnswerOfEverything = 42;
  logFatal() << std::format( "The answer is {}.", theAnswerOfEverything );

  constexpr double someDouble = 4.2;

  const std::tuple tupl { theAnswerOfEverything, 'a', someDouble }; // Another C++17 feature: class template argument deduction
  std::apply( []( auto &&...args ) { logDebug() << std::forward_as_tuple( args... ); ( ( logDebug() << args ), ... ); }, tupl );
  []() { logDebug() << 'a'; logDebug() << 'b'; logDebug() << 'c'; }();

  constexpr auto severities = magic_enum::enum_entries<vx::logger::Path>();
  logFatal() << severities;
  for ( const auto &severity : severities ) {

    logFatal() << severity.first << severity.second;
  }
  logDebug() << magic_enum::enum_name( vx::logger::Path::Filename ) << vx::logger::Path::Filename;

  constexpr auto severityNames = magic_enum::enum_names<vx::logger::Path>();
  logInfo() << severityNames;

  enum class Path {

    Filename,
    Complete,
    Absolute
  };

  constexpr auto pathNames = magic_enum::enum_entries<Path>();
  logInfo() << pathNames;

  //  fmt::format("{}", std::vector{'h', 'e', 'l', 'l', 'o'});
  //  log() << std::format( "{}", vec );
  logInfo() << std::format( "int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", theAnswerOfEverything );

  const std::optional<std::string> opti = "myOptional";
  const std::optional<std::string> optiNull = std::nullopt;
  logInfo() << opti << optiNull << std::make_optional<std::vector<char>>( { 'a', 'b', 'c' } );

  //  log() << vx::logger::SourceLocation::Absolute;

  return EXIT_SUCCESS;
}
