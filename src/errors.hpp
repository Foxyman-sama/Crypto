#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <source_location>
#include <sstream>

void throw_exception(const char *const error, const std::source_location &location = std::source_location::current()) {
  std::stringstream ss;
  ss << location.function_name() << ":\n" << error << '\n';
  throw std::runtime_error { ss.str() };
}

inline constexpr const char *const broken_text_error { "Text is broken." };
inline constexpr const char *const key_longer_than_text_error { "Key can't be longer than text." };
inline constexpr const char *const key_contains_non_alphabetic_chars_error {
  "Key contains non-alphabetic characters."
};
inline constexpr const char *const case_is_different_error { "The case is different." };

#endif