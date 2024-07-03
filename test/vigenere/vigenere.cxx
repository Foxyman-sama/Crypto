#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/vigenere_crypto.hpp"

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

TEST(vigenere_encrypt_tests, encrypt_one_word) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.encrypt("ATTACKATDAWN", "LEMON") };

  ASSERT_EQ("LXFOPVEFRNHR", actual);
}

TEST(vigenere_encrypt_tests, encrypt_two_words_and_punct_characters) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.encrypt("HELLO, WORLD!", "BYE") };

  ASSERT_EQ("ICPMM, APPPE!", actual);
}

TEST(vigenere_encrypt_tests, error_when_key_contains_not_only_alphabet) {
  VigenereCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.encrypt("HELLO, WORLD!", "BYE!"));
}

TEST(vigenere_encrypt_tests, error_when_key_is_longet_than_text) {
  VigenereCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.encrypt("BYE", "HELLO"));
}

TEST(vigenere_decrypt_tests, decrypt_one_word) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.decrypt("LXFOPVEFRNHR", "LEMON") };

  ASSERT_EQ("ATTACKATDAWN", actual);
}

TEST(vigenere_decrypt_tests, decrypt_two_words_and_punct_characters) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.decrypt("ICPMM, APPPE!", "BYE") };

  ASSERT_EQ("HELLO, WORLD!", actual);
}

TEST(vigenere_decrypt_tests, error_when_key_contains_not_only_alphabet) {
  VigenereCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.decrypt("ICPMM, APPPE!", "BYE!"));
}

TEST(vigenere_decrypt_tests, error_when_key_is_longet_than_text) {
  VigenereCryptoStrategy crypto;

  ASSERT_ANY_THROW(crypto.decrypt("BYE", "ICPMM, APPPE!"));
}

TEST(vigenere_encrypt_tests, encrypt_lower_case_phrase) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.encrypt("hello, world!", "bye") };

  ASSERT_EQ("icpmm, apppe!", actual);
}

TEST(vigenere_decrypt_tests, decrypt_lower_case_phrase) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.decrypt("icpmm, apppe!", "bye") };

  ASSERT_EQ("hello, world!", actual);
}