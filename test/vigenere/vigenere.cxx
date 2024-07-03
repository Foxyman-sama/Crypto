#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cctype>

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

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
      throw std::runtime_error { "Key can't be longer than text." };
    }

    for (auto &&ch : key) {
      if (std::isalpha(ch) == false) {
        throw std::runtime_error { "Key contains non-alphabetic characters." };
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

class VigenereCryptoStrategy {
 public:
  std::string encrypt(const std::string &text_for_encoding, const std::any &any) {
    return parse(text_for_encoding, key_parser.parse(text_for_encoding, any),
                 [this](auto text_ch, auto key_ch) { return encrypt_char(text_ch, key_ch); });
  }

  std::string decrypt(const std::string &text_for_decoding, const std::any &any) {
    return parse(text_for_decoding, key_parser.parse(text_for_decoding, any),
                 [this](auto text_ch, auto key_ch) { return decrypt_char(text_ch, key_ch); });
  }

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
    const auto encrypted { (text_ch - 'A' + key_ch - 'A') % 26 + 'A' };
    return check_encrypt_boundary(encrypted);
  }

  char check_encrypt_boundary(char encrypted) { return encrypted > 'Z' ? encrypted - 26 : encrypted; }

  char decrypt_char(char text_ch, char key_ch) {
    const auto decrypted { (text_ch - 'A' - key_ch - 'A') % 26 + 'A' };
    return check_decrypt_boundary(decrypted);
  }

  char check_decrypt_boundary(char decrypted) { return decrypted < 'A' ? decrypted + 26 : decrypted; }

  KeyParser key_parser;
};

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
