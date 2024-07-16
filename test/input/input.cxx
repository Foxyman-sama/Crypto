#include "src/input.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/aes_crypto.hpp"
#include "src/caesar_crypto.hpp"
#include "src/input.hpp"
#include "src/vigenere_crypto.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class crypto_input_tests : public Test {
 public:
  DataView data_view;
  CryptoStrategies crypto_strategies;
  std::unique_ptr<CryptoInput> input;

  void SetUp() {
    crypto_strategies["caesar"].reset(new CaesarCryptoStrategy);
    crypto_strategies["vigenere"].reset(new VigenereCryptoStrategy);
    crypto_strategies["aes"].reset(new AESCryptoStrategy);

    input.reset(new CryptoInput { data_view, std::move(crypto_strategies) });
  }
};

TEST_F(crypto_input_tests, caesar_encryption_works) {
  input->encrypt("caesar", "HeLlO, WoRlD", "1");

  ASSERT_EQ("IfMmP, XpSmE", data_view.output_text);
}

TEST_F(crypto_input_tests, vigenere_encryption_works) {
  input->encrypt("vigenere", "HELLO, WORLD!", "BYE");

  ASSERT_EQ("ICPMM, APPPE!", data_view.output_text);
}

TEST_F(crypto_input_tests, aes_encryption_works) {
  input->encrypt("aes", "hellohellohelloh", "hellohellohelloh");

  ASSERT_EQ("28FC955E541068C5E3F60E6505B2EF9E9E2E7847755BE5A404E3D94C05252520", data_view.output_text);
}

TEST_F(crypto_input_tests, data_view_can_contain_exсeptions_text_when_encryption) {
  input->encrypt("caesar", "1", "1");

  ASSERT_THAT(data_view.output_text, HasSubstr(broken_text_error));
}

TEST_F(crypto_input_tests, caesar_decryption_works) {
  input->decrypt("caesar", "IfMmP, XpSmE", "1");

  ASSERT_EQ("HeLlO, WoRlD", data_view.output_text);
}

TEST_F(crypto_input_tests, vigenere_decryption_works) {
  input->decrypt("vigenere", "ICPMM, APPPE!", "BYE");

  ASSERT_EQ("HELLO, WORLD!", data_view.output_text);
}

TEST_F(crypto_input_tests, aes_decryption_works) {
  input->decrypt("aes", "28FC955E541068C5E3F60E6505B2EF9E9E2E7847755BE5A404E3D94C05252520", "hellohellohelloh");

  ASSERT_EQ("hellohellohelloh", data_view.output_text);
}

TEST_F(crypto_input_tests, data_view_can_contain_exсeptions_text_when_decryption) {
  input->decrypt("caesar", "1", "1");

  ASSERT_THAT(data_view.output_text, HasSubstr(broken_text_error));
}