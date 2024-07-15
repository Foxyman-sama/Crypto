#ifndef CRYPTO_STRATEGIES_BINDS_HPP
#define CRYPTO_STRATEGIES_BINDS_HPP

#include <string_view>

inline constexpr std::string_view caesar_crypto_name { "caesar" };
inline constexpr std::string_view vigenere_crypto_name { "vigenere" };
inline constexpr std::string_view aes_crypto_name { "aes" };

#endif