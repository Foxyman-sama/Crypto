#ifndef CRYPTO_STRATEGIES_BINDS_HPP
#define CRYPTO_STRATEGIES_BINDS_HPP

#include <array>
#include <string_view>

constexpr std::array<std::string_view, 3> crypto_strategies_binds { "caesar", "vigenere", "aes" };

#endif