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

#pragma once

/* stl header */
#include <any>
#include <array>
#include <functional>
#include <iomanip>
#include <list>
#include <map>
#include <ostream>
#include <set>
#if defined __GNUC__ && __GNUC__ >= 11 || defined _MSC_VER && _MSC_VER >= 1930 || defined __clang__ && __clang_major__ >= 15
  #include <source_location>
#else
  #include <source_location.hpp>
#endif
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <utility> // std::pair
#include <variant>
#include <vector>

#include <magic_enum.hpp>

/**
 * @todo Configuration LoggerConfiguration::instance oder ähnliches
 * @todo API Dokumentation
 * @todo Switch to filename, /dev/null at runtime
 * @todo Severity eingrenzen - bsp: nur ab error ausgaben
 * @todo Interface für Format bereitstellen - wie sieht ein log eintrag aus, in welchem format wird ein eintrag ausgegeben (z.B. als xml)
 * @todo syslog
 * @todo Windows Log
 * @todo macOS log
 */
namespace vx::logger {

  /**
   * @brief demangle
   * @param _name  R
   * @return R
   * @todo Add even cleaner demangle function, clean up spaces between > > and remove allocator or extra supid unused shit for readability
   */
  std::string demangle( const std::string &_name );

  /**
   * @brief The Severity enum.
   */
  enum class Severity {

    Verbose, /**< Verbose level. */
    Debug,   /**< Debug level. */
    Info,    /**< Info level. */
    Warning, /**< Warning level. */
    Error,   /**< Error level. */
    Fatal    /**< Fatal error level. */
  };

  /**
   * @brief The Flags enum.
   */
  enum class Flags {

    Space, /**< Auto spaces. */
    Quotes /**< Auto qoutes. */
  };

  /**
   * @brief The SourceLocation enum.
   */
  enum class SourceLocation {

    Absolute,    /**< Complete path. */
    Relative,    /**< Absolute path to project. */
    FilenameOnly /**< Only the filename. */
  };

  class Logger {

  public:
    explicit Logger( Severity _severity = Severity::Info,
                     const std::source_location &_location = std::source_location::current() );

    ~Logger();

    /**
     * @brief @~english Delete copy constructor. @~german Entfernt den kopierenden Konstruktor.
     */
    Logger( const Logger & ) = delete;

    /**
     * @brief @~english Delete move constructor. @~german Entfernt den verschobenen Konstruktor.
     */
    Logger( Logger && ) = delete;

    /**
     * @brief @~english Delete copy assign. @~german Entfernt die kopierte Zuweisung.
     * @return @~english Nothing. @~german Keine Rückgabe.
     */
    Logger &operator=( const Logger & ) = delete;

    /**
     * @brief @~english Delete move assign. @~german Entfernt die verschobene Zuweisung.
     * @return @~english Nothing. @~german Keine Rückgabe.
     */
    Logger &operator=( Logger && ) = delete;

    Logger &loggerWithoutDefine() { return *this; }

    Logger &logger() { return *this; }

    Logger &logVerbose() { return *this; }

    Logger &logDebug() { return *this; }

    Logger &logInfo() { return *this; }

    Logger &logWarning() { return *this; }

    Logger &logError() { return *this; }

    Logger &logFatal() { return *this; }

    void printTimestamp();
    void printSeverity( Severity _severity );

    void printString( std::string_view _input );

    inline bool autoSpace() const { return m_autoSpace; }

    inline void setAutoSpace( bool _autoSpace ) { m_autoSpace = _autoSpace; }

    inline bool autoQuotes() const { return m_autoQuotes; }

    inline void setAutoQuotes( bool _autoQuotes ) { m_autoQuotes = _autoQuotes; }

    inline Logger &space() {

      m_autoSpace = true;
      m_stream << ' ';
      return *this;
    }

    inline Logger &nospace() {

      m_autoSpace = false;
      return *this;
    }

    inline Logger &maybeSpace() {

      if ( m_autoSpace ) {

        m_stream << ' ';
      }
      return *this;
    }

    Logger &operator<<( Severity _severity );

    inline Logger &operator<<( bool _input ) {

      m_stream << ( _input ? "true" : "false" );
      return maybeSpace();
    }

    inline Logger &operator<<( char _input ) {

      m_stream << _input;
      return maybeSpace();
    }

    inline Logger &operator<<( int _input ) {

      m_stream << _input;
      return maybeSpace();
    }

#if !defined _MSC_VER || defined _MSC_VER && _MSC_VER >= 1920
    inline Logger &operator<<( unsigned int _input ) {

      m_stream << _input;
      return maybeSpace();
    }
#endif

    inline Logger &operator<<( std::size_t _input ) {

      m_stream << _input;
      return maybeSpace();
    }

    inline Logger &operator<<( float _input ) {

      m_stream << _input;
      return maybeSpace();
    }

    inline Logger &operator<<( double _input ) {

      m_stream << _input;
      return maybeSpace();
    }

    inline Logger &operator<<( const char *_input ) {

      printString( _input );
      return maybeSpace();
    }

    inline Logger &operator<<( std::string_view _input ) {

      printString( _input );
      m_stream << 's' << 'v';
      return maybeSpace();
    }

    inline Logger &operator<<( const std::string &_input ) {

      printString( _input );
      return maybeSpace();
    }

    inline Logger &operator<<( const void *_input ) {

      if ( _input == nullptr ) {

        m_stream << "(nullptr)";
      }
      else {

        m_stream << '(' << _input << ')';
      }
      return maybeSpace();
    }

    inline Logger &operator<<( std::nullptr_t ) {

      m_stream << "(nullptr_t)";
      return maybeSpace();
    }

    std::ostream &stream() { return m_stream; }

    template <typename Tuple>
    void get_impl( [[maybe_unused]] std::size_t _current,
                   [[maybe_unused]] const Tuple &_tuple,
                   [[maybe_unused]] typename std::tuple_size<Tuple>::type _size ) {}

    template <std::size_t _pos, typename Tuple, typename = std::enable_if_t<std::tuple_size<Tuple>::value != _pos>>
    void get_impl( std::size_t _current,
                   const Tuple &_tuple,
                   [[maybe_unused]] std::integral_constant<std::size_t, _pos> _integral ) {

      if ( _current == _pos ) {

        logger() << std::get<_pos>( _tuple );
      }
      else {

        get_impl( _current, _tuple, std::integral_constant<std::size_t, _pos + 1>() );
      }
    }

    template <typename Tuple>
    void get( std::size_t _pos,
              const Tuple &_tuple ) {

      get_impl( _pos, _tuple, std::integral_constant<std::size_t, 0>() );
    }

    template <typename Variant>
    void getV_impl( [[maybe_unused]] std::size_t _current,
                    [[maybe_unused]] const Variant &_variant,
                    [[maybe_unused]] typename std::variant_size<Variant>::type _size ) {}

    template <std::size_t _pos, typename Variant, typename = std::enable_if_t<std::variant_size<Variant>::value != _pos>>
    void getV_impl( std::size_t _current,
                    const Variant &_variant,
                    [[maybe_unused]] std::integral_constant<std::size_t, _pos> _integral ) {

      if ( _current == _pos ) {

        try {

          logger() << std::get<_pos>( _variant );
        }
        catch ( [[maybe_unused]] const std::bad_variant_access &_exception ) {

          /* Nothing to do here. */
        }
      }
      else {

        getV_impl( _current, _variant, std::integral_constant<std::size_t, _pos + 1>() );
      }
    }

    template <typename Variant>
    void getV( std::size_t _pos,
               const Variant &_variant ) {

      getV_impl( _pos, _variant, std::integral_constant<std::size_t, 0>() );
    }

  private:
    bool m_autoSpace = true;
    bool m_autoQuotes = true;

    Severity m_severity = Severity::Info;
    SourceLocation m_sourceLocation = SourceLocation::FilenameOnly;

    std::source_location m_location;
    std::ostream m_stream;
  };

  template <typename Key, typename T>
  inline Logger &operator<<( Logger &_logger,
                             const std::pair<Key, T> &_pair ) {

    const bool saveState = _logger.autoSpace();
    _logger.setAutoQuotes( false );
    _logger.nospace() << "std::pair<" << demangle( typeid( Key ).name() ) << ',' << ' ' << demangle( typeid( T ).name() ) << '>' << ' ' << '{';
    _logger.setAutoQuotes( true );
    _logger << _pair.first << ',' << ' ' << _pair.second;
    _logger << '}';
    _logger.setAutoSpace( saveState );
    return _logger.maybeSpace();
  }

  template <typename List>
  inline Logger &printList( Logger &_logger,
                            const std::string &_what,
                            const List &_list ) {

    using func = std::function<void( void )>;
    func checkComma = []() {};
    func printComma = [ &_logger ]() { _logger << ',' << ' '; };
    func noPrint = [ & ]() { checkComma = printComma; };
    checkComma = noPrint;

    const bool saveState = _logger.autoSpace();
    _logger.setAutoQuotes( false );
    _logger.nospace() << _what << ' ' << '{';
    _logger.setAutoQuotes( true );
    for ( const auto &value : _list ) {

      checkComma();
      _logger << value;
    }
    _logger << '}';
    _logger.setAutoSpace( saveState );
    return _logger.maybeSpace();
  }

  template <typename T, std::size_t N>
  inline Logger &operator<<( Logger &_logger,
                             const std::array<T, N> &_array ) {

    return printList( _logger, "std::array<" + demangle( typeid( T ).name() ) + ", " + std::to_string( N ) + ">", _array );
  }

  template <typename T, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::list<T, Alloc> &_list ) {

    return printList( _logger, "std::list<" + demangle( typeid( T ).name() ) + ">", _list );
  }

  template <typename T, typename Compare, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::set<T, Compare, Alloc> &_set ) {

    return printList( _logger, "std::set<" + demangle( typeid( T ).name() ) + ">", _set );
  }

  template <typename T, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::vector<T, Alloc> &_vector ) {

    return printList( _logger, "std::vector<" + demangle( typeid( T ).name() ) + ">", _vector );
  }

  template <typename T>
  inline Logger &printMap( Logger &_logger,
                           const std::string &_what,
                           const T &_map ) {

    using func = std::function<void( void )>;
    func checkComma = []() {};
    func printComma = [ &_logger ]() { _logger << ',' << ' '; };
    func noPrint = [ & ]() { checkComma = printComma; };
    checkComma = noPrint;

    const bool saveState = _logger.autoSpace();
    _logger.setAutoQuotes( false );
    _logger.nospace() << _what << ' ' << '{';
    _logger.setAutoQuotes( true );
    for ( const auto &[ key, value ] : _map ) {

      checkComma();
      _logger << '{' << key << ',' << ' ' << value << '}';
    }
    _logger << '}';
    _logger.setAutoSpace( saveState );
    return _logger.maybeSpace();
  }

  template <typename Key, typename T, typename Compare, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::map<Key, T, Compare, Alloc> &_map ) {

    return printMap( _logger, "std::map<" + demangle( typeid( Key ).name() ) + ", " + demangle( typeid( T ).name() ) + ">", _map );
  }

  template <typename Key, typename T, typename Compare, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::multimap<Key, T, Compare, Alloc> &_map ) {

    return printMap( _logger, "std::multimap<" + demangle( typeid( Key ).name() ) + ", " + demangle( typeid( T ).name() ) + ">", _map );
  }

  template <typename Key, typename T, typename Compare, typename Alloc>
  inline Logger &operator<<( Logger &_logger,
                             const std::unordered_map<Key, T, Compare, Alloc> &_map ) {

    return printMap( _logger, "std::unordered_map<" + demangle( typeid( Key ).name() ) + ", " + demangle( typeid( T ).name() ) + ">", _map );
  }

  template <typename T>
  inline Logger &printTuple( Logger &_logger,
                             const std::string &_what,
                             const T &_tuple ) {

    using func = std::function<void( void )>;
    func checkComma = []() {};
    func printComma = [ &_logger ]() { _logger << ',' << ' '; };
    func noPrint = [ & ]() { checkComma = printComma; };
    checkComma = noPrint;

    const bool saveState = _logger.autoSpace();
    _logger.setAutoQuotes( false );
    _logger.nospace() << _what << ' ' << '{';
    _logger.setAutoQuotes( true );
    std::size_t tupleSize = std::tuple_size_v<T>;
    for ( std::size_t pos = 0; pos < tupleSize; pos++ ) {

      checkComma();
      _logger.get( pos, _tuple );
    }
    _logger << '}';
    _logger.setAutoSpace( saveState );
    return _logger.maybeSpace();
  }

  template <typename... Types>
  inline Logger &operator<<( Logger &_logger,
                             const std::tuple<Types...> &_values ) {

    return printTuple( _logger, demangle( typeid( std::tuple<Types...> ).name() ), _values );
  }

  template <typename T>
  inline Logger &printVariant( Logger &_logger,
                               const std::string &_what,
                               const T &_variant ) {

    const bool saveState = _logger.autoSpace();
    _logger.setAutoQuotes( false );
    _logger.nospace() << _what << ' ';
    _logger.setAutoQuotes( true );
    std::size_t variantSize = std::variant_size_v<T>;
    for ( std::size_t pos = 0; pos < variantSize; pos++ ) {

      _logger.getV( pos, _variant );
    }
    _logger.setAutoSpace( saveState );
    return _logger.maybeSpace();
  }

  template <typename... Types>
  inline Logger &operator<<( Logger &_logger,
                             const std::variant<Types...> &_values ) {

    return printVariant( _logger, demangle( typeid( std::variant<Types...> ).name() ), _values );
  }

  template <typename Type, typename Function>
  inline std::pair<const std::type_index, std::function<void( Logger &, const std::any & )>> to_any_visitor( const Function &_function ) {

    return {

      std::type_index( typeid( Type ) ),
      [ function = _function ]( Logger &_logger, [[maybe_unused]] const std::any &_any ) {
        function( _logger, std::any_cast<const Type &>( _any ) );
      }
    };
  }

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wexit-time-destructors"
  #pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
  static std::unordered_map<std::type_index, std::function<void( Logger &_logger, const std::any & )>> any_visitor {

    to_any_visitor<bool>( []( Logger &_logger, bool _input ) { _logger << _input; } ),
    to_any_visitor<char>( []( Logger &_logger, char _input ) { _logger << _input; } ),
    to_any_visitor<int>( []( Logger &_logger, int _input ) { _logger << _input; } ),
    to_any_visitor<unsigned int>( []( Logger &_logger, unsigned int _input ) { _logger << _input; } ),
    to_any_visitor<std::size_t>( []( Logger &_logger, std::size_t _input ) { _logger << _input; } ),
    to_any_visitor<float>( []( Logger &_logger, float _input ) { _logger << _input; } ),
    to_any_visitor<double>( []( Logger &_logger, double _input ) { _logger << _input; } ),
    to_any_visitor<const char *>( []( Logger &_logger, const char *_input ) { _logger << _input; } ),
    to_any_visitor<std::string_view>( []( Logger &_logger, std::string_view _input ) { _logger << _input; } ),
    to_any_visitor<std::string>( []( Logger &_logger, const std::string &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<bool>>( []( Logger &_logger, const std::list<bool> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<char>>( []( Logger &_logger, const std::list<char> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<int>>( []( Logger &_logger, const std::list<int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<unsigned int>>( []( Logger &_logger, const std::list<unsigned int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<std::size_t>>( []( Logger &_logger, const std::list<std::size_t> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<float>>( []( Logger &_logger, const std::list<float> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<double>>( []( Logger &_logger, const std::list<double> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<const char *>>( []( Logger &_logger, const std::list<const char *> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<std::string_view>>( []( Logger &_logger, const std::list<std::string_view> &_input ) { _logger << _input; } ),
    to_any_visitor<std::list<std::string>>( []( Logger &_logger, const std::list<std::string> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<bool>>( []( Logger &_logger, const std::set<bool> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<char>>( []( Logger &_logger, const std::set<char> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<int>>( []( Logger &_logger, const std::set<int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<unsigned int>>( []( Logger &_logger, const std::set<unsigned int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<std::size_t>>( []( Logger &_logger, const std::set<std::size_t> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<float>>( []( Logger &_logger, const std::set<float> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<double>>( []( Logger &_logger, const std::set<double> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<const char *>>( []( Logger &_logger, const std::set<const char *> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<std::string_view>>( []( Logger &_logger, const std::set<std::string_view> &_input ) { _logger << _input; } ),
    to_any_visitor<std::set<std::string>>( []( Logger &_logger, const std::set<std::string> &_input ) { _logger << _input; } ),
    // MAC found that ambiguous...    to_any_visitor<std::vector<bool>>( []( Logger & _logger, const std::vector<bool> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<char>>( []( Logger &_logger, const std::vector<char> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<int>>( []( Logger &_logger, const std::vector<int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<unsigned int>>( []( Logger &_logger, const std::vector<unsigned int> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<std::size_t>>( []( Logger &_logger, const std::vector<std::size_t> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<float>>( []( Logger &_logger, const std::vector<float> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<double>>( []( Logger &_logger, const std::vector<double> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<const char *>>( []( Logger &_logger, const std::vector<const char *> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<std::string_view>>( []( Logger &_logger, const std::vector<std::string_view> &_input ) { _logger << _input; } ),
    to_any_visitor<std::vector<std::string>>( []( Logger &_logger, const std::vector<std::string> &_input ) { _logger << _input; } ),
    // VC2017 issue    to_any_visitor<void>( []( Logger & _logger, [[maybe_unused]] void *_input ) { _logger << _input; } ),
  };
#ifdef __clang__
  #pragma clang diagnostic pop
#endif

  inline void process( Logger &_logger,
                       const std::any &_any ) {

    if ( const auto iterator = any_visitor.find( std::type_index( _any.type() ) ); iterator != any_visitor.cend() ) {

      iterator->second( _logger, _any );
    }
    else {

      _logger << "Unregistered type:" << demangle( _any.type().name() );
    }
  }

  template <typename Type, typename Function>
  inline void register_any_visitor( const Function &_function ) {

    any_visitor.insert( to_any_visitor<Type>( _function ) );
  }

  inline Logger &operator<<( Logger &_logger,
                             const std::any &_input ) {

    process( _logger, _input );
    return _logger.maybeSpace();
  }

  template <typename Char, typename Traits, typename E, magic_enum::detail::enable_if_t<E, int> = 0>
  inline Logger &operator<<( Logger &_logger, E value ) {

    using D = std::decay_t<E>;
    using U = magic_enum::underlying_type_t<D>;

    if constexpr ( magic_enum::detail::supported<D>::value ) {

      if ( const auto name = enum_flags_name<D>( value ); !name.empty() ) {

        for ( const auto c : name ) {

          _logger << c;
        }
        return _logger.maybeSpace();
      }
    }
    return ( _logger << static_cast<U>( value ) );
  }

  template <typename Char, typename Traits, typename E, magic_enum::detail::enable_if_t<E, int> = 0>
  inline Logger &operator<<( Logger &_logger, magic_enum::optional<E> value ) {

    return value ? ( _logger << *value ) : _logger;
  }
}

//static inline vx::logger::Logger &log2() { return vx::logger::Logger().logDebug(); }

#define log vx::logger::Logger().logger
#define verbose vx::logger::Logger( vx::logger::Severity::Verbose ).logVerbose
#define debug vx::logger::Logger( vx::logger::Severity::Debug ).logDebug
#define info vx::logger::Logger( vx::logger::Severity::Info ).logInfo
#define warning vx::logger::Logger( vx::logger::Severity::Warning ).logWarning
#define error vx::logger::Logger( vx::logger::Severity::Error ).logError
#define fatal vx::logger::Logger( vx::logger::Severity::Fatal ).logFatal
