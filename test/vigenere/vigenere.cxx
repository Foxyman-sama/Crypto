#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cctype>

int main() {
  testing::InitGoogleTest();
  testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}

class VigenereCryptoStrategy {
 public:
  std::string encrypt(const std::string &text_for_encoding, const std::any &any) {
    return parse(text_for_encoding, parse_key(text_for_encoding, any));
  }

 private:
  std::string parse_key(const std::string &text_for_encoding, const std::any &any) {
    const auto key { extract_key(any) };
    check_key(text_for_encoding, key);
    return match_key_to_text(text_for_encoding, key);
  }

  std::string extract_key(const std::any &any) { return std::any_cast<const char *>(any); }

  void check_key(const std::string &text_for_encoding, const std::string &key) {
    if (text_for_encoding.length() < key.length()) {
      throw std::runtime_error { "Key can't be longer than text." };
    }

    for (auto &&ch : key) {
      if (std::isalpha(ch) == false) {
        throw std::runtime_error { "Key contains non-alphabetic characters." };
      }
    }
  }

  std::string match_key_to_text(const std::string &text_for_encoding, const std::string &key) {
    const auto text_length { text_for_encoding.length() };
    const auto key_length { key.length() };

    std::string result;

    for (int i = 0, j = 0; i < text_length; ++i) {
      const char text_ch { text_for_encoding[i] };
      if (std::isspace(text_ch) || std::ispunct(text_ch)) {
        result += text_ch;
      } else {
        result += key[j++ % key_length];
      }
    }

    return result;
  }

  std::string parse(const std::string &text_for_encoding, const std::string &key) {
    std::string result;

    for (int i = 0; i < text_for_encoding.length(); ++i) {
      result += match_character(text_for_encoding[i], key[i]);
    }

    return result;
  }

  char match_character(const char text_ch, const char key_ch) {
    if (std::ispunct(text_ch) || std::isspace(text_ch)) {
      return text_ch;
    } else {
      return encrypt_char(text_ch, key_ch);
    }
  }

  char encrypt_char(char text_ch, char key_ch) {
    const auto encrypted { (text_ch - 'A' + key_ch - 'A') % 26 + 'A' };
    return check_encrypt_boundary(encrypted);
  }

  char check_encrypt_boundary(char encrypted) { return encrypted > 'Z' ? encrypted - 26 : encrypted; }
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