#ifndef NOSTD_SOURCE_LOCATION_HPP
#define NOSTD_SOURCE_LOCATION_HPP

#pragma once

#include <cstdint>

namespace std {
struct source_location {
public:
#ifdef _WIN32
  static constexpr source_location current(const char* fileName = "unsupported",
      const char* functionName = "unsupported",
      const uint_least32_t lineNumber = 0,
      const uint_least32_t columnOffset = 0) noexcept
#else
#if defined(__clang__) and (__clang_major__ >= 9)
    static constexpr source_location current(const char* fileName = __builtin_FILE(),
        const char* functionName = __builtin_FUNCTION(),
        const uint_least32_t lineNumber = __builtin_LINE(),
        const uint_least32_t columnOffset = __builtin_COLUMN()) noexcept
#elif defined(__GNUC__) and (__GNUC__ > 4 or (__GNUC__ == 4 and __GNUC_MINOR__ >= 8))
    static constexpr source_location current(const char* fileName = __builtin_FILE(),
        const char* functionName = __builtin_FUNCTION(),
        const uint_least32_t lineNumber = __builtin_LINE(),
        const uint_least32_t columnOffset = 0) noexcept
#else
    static constexpr source_location current(const char* fileName = "unsupported",
        const char* functionName = "unsupported",
        const uint_least32_t lineNumber = 0,
        const uint_least32_t columnOffset = 0) noexcept
#endif
#endif
    {
        return {fileName, functionName, lineNumber, columnOffset};
    }

    source_location(const source_location&) = default;
    source_location(source_location&&) = default;

    [[nodiscard]] constexpr const char* file_name() const noexcept
    {
        return fileName;
    }

    [[nodiscard]] constexpr const char* function_name() const noexcept
    {
        return functionName;
    }

    [[nodiscard]] constexpr uint_least32_t line() const noexcept
    {
        return lineNumber;
    }

    [[nodiscard]] constexpr std::uint_least32_t column() const noexcept
    {
        return columnOffset;
    }

private:
    constexpr source_location(const char* _fileName, const char* _functionName, const uint_least32_t _lineNumber,
        const uint_least32_t _columnOffset) noexcept
        : fileName(_fileName)
        , functionName(_functionName)
        , lineNumber(_lineNumber)
        , columnOffset(_columnOffset)
    {
    }

    const char* fileName;
    const char* functionName;
    const std::uint_least32_t lineNumber;
    const std::uint_least32_t columnOffset;
};
} // namespace nostd

#endif
