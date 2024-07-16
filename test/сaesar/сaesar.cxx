#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/caesar_crypto.hpp"

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

TEST(caesar_encrypt_tests, encrypt_different_case_with_single_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HeLlO, WoRlD", 1) };

  ASSERT_EQ("IfMmP, XpSmE", actual);
}

TEST(caesar_encrypt_tests, encrypt_different_case_with_twenty_six_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HeLlO, WoRlD", 26) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}

TEST(caesar_encrypt_tests, encrypt_different_case_with_twenty_five_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HeLlO, WoRlD", 25) };

  ASSERT_EQ("GdKkN, VnQkC", actual);
}

TEST(caesar_exception_tests, throw_exception_when_str_contain_something_else) {
  CaesarCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.encrypt("1", 1));
}

TEST(caesar_decrypt_tests, decrypt_different_case_with_single_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.decrypt("IfMmP, XpSmE", 1) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}

TEST(caesar_decrypt_tests, decrypt_different_case_with_twenty_six_shift) {
  CaesarCryptoStrategy crypto;

  const auto actual { crypto.decrypt("HeLlO, WoRlD", 26) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}
