#include <iostream>

#include "caesar_crypto.hpp"
#include "window.hpp"

class Fake : public Input {
 public:
  void encrypt(const std::string &crypto_name, const std::string &text_for_encoding, const std::string &key) override {}

 private:
  CaesarCryptoStrategy crypto;
};

int main() {
  Fake fake;
  Window win { fake };
  win.show("Crypto", 800, 600);
}