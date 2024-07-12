#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/aes_crypto.hpp"

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

TEST(aes_encrypt_tests, encrypt_one_word) {
  AESCryptoStrategy crypto;

  const auto actual { crypto.encrypt("hellohellohelloh", "hellohellohelloh") };

  ASSERT_EQ("28FC955E541068C5E3F60E6505B2EF9E9E2E7847755BE5A404E3D94C05252520", actual);
}

TEST(aes_encrypt_tests, error_when_key_is_so_short) {
  AESCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.encrypt("hellohellohelloh", "hellohellohellh"));
}

TEST(aes_encrypt_tests, encrypt_different_case) {
  AESCryptoStrategy crypto;

  const auto actual { crypto.encrypt("Hello, World!", "hellohellohelloh") };

  ASSERT_EQ("2194DE9B8F7D945524307B05D0561AF8", actual);
}

TEST(aes_decrypt_tests, decrypt_one_word) {
  AESCryptoStrategy crypto;

  const auto actual { crypto.decrypt("28FC955E541068C5E3F60E6505B2EF9E9E2E7847755BE5A404E3D94C05252520",
                                     "hellohellohelloh") };

  ASSERT_EQ("hellohellohelloh", actual);
}

TEST(aes_decrypt_tests, error_when_key_is_so_short) {
  AESCryptoStrategy crypto;

  ASSERT_ANY_THROW(
      crypto.decrypt("28FC955E541068C5E3F60E6505B2EF9E9E2E7847755BE5A404E3D94C05252520", "hellohellohello"));
}