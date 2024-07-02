#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cctype>

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

class CaesarCryptoStrategy {
 public:
  std::string encrypt(const std::string &str, int shift) {
    std::string result;

    for (auto &&ch : str) {
      result += parse_character(ch, shift);
    }

    return result;
  }

  std::string decrypt(const std::string &str, int shift) { return encrypt(str, 26 - shift); }

 private:
  char parse_character(unsigned char ch, int shift) {
    if (std::ispunct(ch) || std::isspace(ch)) {
      return ch;
    } else {
      return parse_as_alphabet(ch, shift);
    }
  }

  char parse_as_alphabet(unsigned char ch, int shift) {
    auto encrypted { ch + shift % 26 };
    if (ch >= 'a' && ch <= 'z') {
      return check_boundary_lower_case(encrypted);
    } else if (ch >= 'A' && ch <= 'Z') {
      return check_boundary_upper_case(encrypted);
    } else {
      throw std::runtime_error { "Unknown character." };
    }
  }

  char check_boundary_lower_case(char encrypted) { return encrypted > 'z' ? encrypted - 'z' + 'a' - 1 : encrypted; }

  char check_boundary_upper_case(char encrypted) { return encrypted > 'Z' ? encrypted - 'Z' + 'A' - 1 : encrypted; }
};

TEST(caesar_encrypt_tests, encrypt_lower_case_with_single_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("hello, world", 1) };

  ASSERT_EQ("ifmmp, xpsme", actual);
}

TEST(caesar_encrypt_tests, encrypt_different_case_with_single_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HeLlO, WoRlD", 1) };

  ASSERT_EQ("IfMmP, XpSmE", actual);
}

TEST(caesar_encrypt_tests, encrypt_lower_case_with_twenty_six_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("hello, world", 26) };

  ASSERT_EQ("hello, world", actual);
}

TEST(caesar_encrypt_tests, encrypt_different_case_with_twenty_six_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HeLlO, WoRlD", 26) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}

TEST(caesar_exception_tests, throw_exception_when_str_contain_something_else) {
  CaesarCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.encrypt("1", 1));
}

TEST(caesar_decrypt_tests, decrypt_lower_case) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.decrypt("ifmmp, xpsme", 1) };

  ASSERT_EQ("hello, world", actual);
}
