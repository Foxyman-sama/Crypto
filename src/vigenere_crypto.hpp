#ifndef VIGENERE_CRYPTO_HPP
#define VIGENERE_CRYPTO_HPP

#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>

#include "crypto_strategy.hpp"
#include "errors.hpp"

class KeyParser {
 public:
  std::string parse(const std::string &text, const std::any &any) {
    const auto key { extract_key(any) };
    check_key(text, key);
    return match_key_to_text(text, key);
  }

 private:
  std::string extract_key(const std::any &any) { return std::any_cast<const char *>(any); }

  void check_key(const std::string &text, const std::string &key) {
    if (text.length() < key.length()) {
      throw_exception(key_longer_than_text_error);
    }

    for (auto &&ch : key) {
      if (std::isalpha(ch) == false) {
        throw_exception(key_contains_non_alphabetic_chars_error);
      }
    }
  }

  std::string match_key_to_text(const std::string &text, const std::string &key) {
    const auto text_length { text.length() };
    const auto key_length { key.length() };

    std::string result;

    for (int i = 0, j = 0; i < text_length; ++i) {
      const char text_ch { text[i] };
      if (std::isspace(text_ch) || std::ispunct(text_ch)) {
        result += text_ch;
      } else {
        result += key[j++ % key_length];
      }
    }

    return result;
  }
};

class VigenereCryptoStrategy : public CryptoStrategy {
 public:
  std::string encrypt(const std::string &text_for_encoding, const std::any &any) override {
    return parse(text_for_encoding, key_parser.parse(text_for_encoding, any),
                 [this](auto text_ch, auto key_ch) { return encrypt_char(text_ch, key_ch); });
  }

  std::string decrypt(const std::string &text_for_decoding, const std::any &any) override {
    return parse(text_for_decoding, key_parser.parse(text_for_decoding, any),
                 [this](auto text_ch, auto key_ch) { return decrypt_char(text_ch, key_ch); });
  }

  bool is_key_numeric() noexcept override { return false; }

 private:
  std::string parse(const std::string &text_for_encoding, const std::string &key,
                    std::function<char(char, char)> policy) {
    const auto text_length { text_for_encoding.length() };

    std::string result;

    for (int i = 0; i < text_length; ++i) {
      result += parse_character(text_for_encoding[i], key[i], policy);
    }

    return result;
  }

  char parse_character(const char text_ch, const char key_ch, std::function<char(char, char)> policy) {
    if (std::ispunct(text_ch) || std::isspace(text_ch)) {
      return text_ch;
    } else {
      return policy(text_ch, key_ch);
    }
  }

  char encrypt_char(char text_ch, char key_ch) {
    if (text_ch >= 'a' && text_ch <= 'z') {
      const auto encrypted { ((text_ch - 'a') + (key_ch - 'a')) % 26 + 'a' };
      return check_encrypt_boundary_lower_case(encrypted);
    } else if (text_ch >= 'A' && text_ch <= 'Z') {
      const auto encrypted { ((text_ch - 'A') + (key_ch - 'A')) % 26 + 'A' };
      return check_encrypt_boundary_upper_case(encrypted);
    } else {
      throw_exception(broken_text_error);
    }
  }

  char check_encrypt_boundary_lower_case(char encrypted) { return encrypted > 'z' ? encrypted - 26 : encrypted; }

  char check_encrypt_boundary_upper_case(char encrypted) { return encrypted > 'Z' ? encrypted - 26 : encrypted; }

  char decrypt_char(char text_ch, char key_ch) {
    if (text_ch >= 'a' && text_ch <= 'z') {
      const auto decrypted { ((text_ch - 'a') - (key_ch - 'a')) % 26 + 'a' };
      return check_decrypt_boundary_lower_case(decrypted);
    } else if (text_ch >= 'A' && text_ch <= 'Z') {
      const auto decrypted { ((text_ch - 'A') - (key_ch - 'A')) % 26 + 'A' };
      return check_decrypt_boundary_upper_case(decrypted);
    } else {
      throw_exception(broken_text_error);
    }
  }

  char check_decrypt_boundary_lower_case(char decrypted) { return decrypted < 'a' ? decrypted + 26 : decrypted; }

  char check_decrypt_boundary_upper_case(char decrypted) { return decrypted < 'A' ? decrypted + 26 : decrypted; }

  KeyParser key_parser;
};

#endif