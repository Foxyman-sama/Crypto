#include <gmock/gmock.h>
#include <gtest/gtest.h>

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

class VigenereCryptoStrategy {
 public:
  std::string encrypt(const std::string &str, const std::any &key) { return parse(str, make_key(str, key)); }

 private:
  std::string make_key(const std::string &str, const std::any &key) {
    std::string result { std::any_cast<const char *>(key) };

    for (int i = result.length(), j = 0; i < str.length(); ++i, ++j) {
      result += result[j];
    }

    return result;
  }

  std::string parse(const std::string &str, const std::string &key) {
    std::string result;

    for (int i = 0; i < str.length(); ++i) {
      result += encrypt_char(str[i], key[i]);
    }

    return result;
  }

  char encrypt_char(char str_ch, char key_ch) {
    const auto encrypted { (str_ch - 'A' + key_ch - 'A') % 26 + 'A' };
    return check_encrypt_boundary(encrypted);
  }

  char check_encrypt_boundary(char encrypted) { return encrypted > 'Z' ? encrypted - 26 : encrypted; }
};

TEST(vigenere_encrypt_tests, encrypt_one_word) {
  VigenereCryptoStrategy crypto;

  const auto actual { crypto.encrypt("ATTACKATDAWN", "LEMON") };

  ASSERT_EQ("LXFOPVEFRNHR", actual);
}