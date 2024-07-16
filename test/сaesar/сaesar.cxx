#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/caesar_crypto.hpp"

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

class caesar_tests : public testing::Test {
 public:
  CaesarCryptoStrategy crypto;
};

class caesar_encrypt_tests : public caesar_tests {};

class caesar_decrypt_tests : public caesar_tests {};

TEST_F(caesar_encrypt_tests, encrypt_different_case_with_single_shift) {
  const auto actual { crypto.encrypt("HeLlO, WoRlD", 1) };

  ASSERT_EQ("IfMmP, XpSmE", actual);
}

TEST_F(caesar_encrypt_tests, encrypt_different_case_with_twenty_six_shift) {
  const auto actual { crypto.encrypt("HeLlO, WoRlD", 26) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}

TEST_F(caesar_encrypt_tests, encrypt_different_case_with_twenty_five_shift) {
  const auto actual { crypto.encrypt("HeLlO, WoRlD", 25) };

  ASSERT_EQ("GdKkN, VnQkC", actual);
}

TEST_F(caesar_encrypt_tests, throw_exception_when_str_contain_something_else) {
  ASSERT_ANY_THROW(crypto.encrypt("1", 1));
}

TEST_F(caesar_decrypt_tests, decrypt_different_case_with_single_shift) {
  const auto actual { crypto.decrypt("IfMmP, XpSmE", 1) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}

TEST_F(caesar_decrypt_tests, decrypt_different_case_with_twenty_six_shift) {
  const auto actual { crypto.decrypt("HeLlO, WoRlD", 26) };

  ASSERT_EQ("HeLlO, WoRlD", actual);
}
