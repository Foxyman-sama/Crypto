#include <iostream>

#include "aes_crypto.hpp"
#include "caesar_crypto.hpp"
#include "crypto_strategies_binds.hpp"
#include "input.hpp"
#include "vigenere_crypto.hpp"
#include "window.hpp"

int main() {
  DataView data_view;

  CryptoStrategies crypto_strategies;
  crypto_strategies[crypto_strategies_binds[0]].reset(new CaesarCryptoStrategy);
  crypto_strategies[crypto_strategies_binds[1]].reset(new VigenereCryptoStrategy);
  crypto_strategies[crypto_strategies_binds[2]].reset(new AESCryptoStrategy);

  CryptoInput input { data_view, std::move(crypto_strategies) };
  Window win { input, data_view };
  win.show("Crypto", 640, 480);
}