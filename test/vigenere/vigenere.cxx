#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/vigenere_crypto.hpp"

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

class vigenere_tests : public testing::Test {
 public:
  VigenereCryptoStrategy crypto;
};

class vigenere_encrypt_tests : public vigenere_tests {};

class vigenere_decrypt_tests : public vigenere_tests {};

TEST_F(vigenere_encrypt_tests, encrypt_one_word) {
  const auto actual { crypto.encrypt("ATTACKATDAWN", "LEMON") };

  ASSERT_EQ("LXFOPVEFRNHR", actual);
}

TEST_F(vigenere_encrypt_tests, encrypt_two_words_and_punct_characters) {
  const auto actual { crypto.encrypt("HELLO, WORLD!", "BYE") };

  ASSERT_EQ("ICPMM, APPPE!", actual);
}

TEST_F(vigenere_encrypt_tests, error_when_key_contains_not_only_alphabet) {
  ASSERT_ANY_THROW(crypto.encrypt("HELLO, WORLD!", "BYE!"));
}

TEST_F(vigenere_encrypt_tests, error_when_key_is_longet_than_text) { ASSERT_ANY_THROW(crypto.encrypt("BYE", "HELLO")); }

TEST_F(vigenere_decrypt_tests, decrypt_one_word) {
  const auto actual { crypto.decrypt("LXFOPVEFRNHR", "LEMON") };

  ASSERT_EQ("ATTACKATDAWN", actual);
}

TEST_F(vigenere_decrypt_tests, decrypt_two_words_and_punct_characters) {
  const auto actual { crypto.decrypt("ICPMM, APPPE!", "BYE") };

  ASSERT_EQ("HELLO, WORLD!", actual);
}

TEST_F(vigenere_decrypt_tests, error_when_key_contains_not_only_alphabet) {
  ASSERT_ANY_THROW(crypto.decrypt("ICPMM, APPPE!", "BYE!"));
}

TEST_F(vigenere_decrypt_tests, error_when_key_is_longet_than_text) {
  ASSERT_ANY_THROW(crypto.decrypt("BYE", "ICPMM, APPPE!"));
}

TEST_F(vigenere_encrypt_tests, encrypt_lower_case_phrase) {
  const auto actual { crypto.encrypt("hello, world!", "bye") };

  ASSERT_EQ("icpmm, apppe!", actual);
}

TEST_F(vigenere_decrypt_tests, decrypt_lower_case_phrase) {
  const auto actual { crypto.decrypt("icpmm, apppe!", "bye") };

  ASSERT_EQ("hello, world!", actual);
}