#ifndef CAESAR_CRYPTO_HPP
#define CAESAR_CRYPTO_HPP

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>

class CaesarCryptoStrategy {
 public:
  std::string encrypt(const std::string &text_for_encoding, std::any any) {
    return parse(text_for_encoding, std::any_cast<int>(any),
                 [this](auto ch, auto shift) { return encrypt_char(ch, shift); });
  }

  std::string decrypt(const std::string &text_for_encoding, std::any any) {
    return parse(text_for_encoding, std::any_cast<int>(any),
                 [this](auto ch, auto shift) { return decrypt_char(ch, shift); });
  }

 private:
  std::string parse(const std::string &text_for_encoding, int shift, std::function<char(char, int)> policy) {
    std::string result;

    for (auto &&ch : text_for_encoding) {
      result += parse_character(ch, shift, policy);
    }

    return result;
  }

  char parse_character(char ch, int shift, std::function<char(char, int)> policy) {
    if (std::ispunct(ch) || std::isspace(ch)) {
      return ch;
    } else {
      return policy(ch, shift);
    }
  }

  char encrypt_char(char ch, int shift) {
    auto encrypted { ch + shift % 26 };
    if (ch >= 'a' && ch <= 'z') {
      return check_encrypt_boundary_lower_case(encrypted);
    } else if (ch >= 'A' && ch <= 'Z') {
      return check_encrypt_boundary_upper_case(encrypted);
    } else {
      throw std::runtime_error { "Unknown character." };
    }
  }

  char check_encrypt_boundary_lower_case(char encrypted) {
    return encrypted > 'z' ? encrypted - 'z' + 'a' - 1 : encrypted;
  }

  char check_encrypt_boundary_upper_case(char encrypted) {
    return encrypted > 'Z' ? encrypted - 'Z' + 'A' - 1 : encrypted;
  }

  char decrypt_char(char ch, int shift) {
    auto decrypted { ch - shift % 26 };
    if (ch >= 'a' && ch <= 'z') {
      return check_decrypt_boundary_lower_case(decrypted);
    } else if (ch >= 'A' && ch <= 'Z') {
      return check_decrypt_boundary_upper_case(decrypted);
    } else {
      throw std::runtime_error { "Unknown character." };
    }
  }

  char check_decrypt_boundary_lower_case(char decrypted) {
    return decrypted < 'a' ? decrypted + 'z' - 'a' + 1 : decrypted;
  }

  char check_decrypt_boundary_upper_case(char decrypted) {
    return decrypted < 'A' ? decrypted + 'Z' - 'A' + 1 : decrypted;
  }
};

#endif