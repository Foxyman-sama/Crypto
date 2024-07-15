#ifndef INPUT_HPP
#define INPUT_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "crypto_strategy.hpp"
#include "data_view.hpp"

using CryptoStrategies = std::unordered_map<std::string, std::unique_ptr<CryptoStrategy>>;

class Input {
 public:
  virtual void encrypt(const std::string &crypto_name, const std::string &text_for_encoding,
                       const std::string &key) = 0;
};

class CryptoInput {
 public:
  CryptoInput(DataView &data_view, CryptoStrategies &&crypto_strategies)
      : data_view { data_view }, crypto_strategies { std::move(crypto_strategies) } {}

  void encrypt(const std::string &crypto_strategy_name, const std::string &text_for_encoding, const char *key) {
    auto &strategy { get_strategy(crypto_strategy_name) };
    if (strategy.is_key_numeric()) {
      encrypt_when_numeric_key(strategy, text_for_encoding, key);
    } else {
      encrypt_when_non_numeric_key(strategy, text_for_encoding, key);
    }
  }

 private:
  CryptoStrategy &get_strategy(const std::string &crypto_strategy_name) {
    return *crypto_strategies[crypto_strategy_name];
  }

  void encrypt_when_numeric_key(CryptoStrategy &strategy, const std::string &text_for_encoding, const char *key) {
    data_view.output_text = strategy.encrypt(text_for_encoding, std::stoi(key));
  }

  void encrypt_when_non_numeric_key(CryptoStrategy &strategy, const std::string &text_for_encoding, const char *key) {
    data_view.output_text = strategy.encrypt(text_for_encoding, key);
  }

  DataView &data_view;
  CryptoStrategies crypto_strategies;
};

#endif