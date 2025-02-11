#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "crypto_strategy.hpp"
#include "data_view.hpp"

using CryptoStrategies = std::unordered_map<std::string_view, std::unique_ptr<CryptoStrategy>>;

class Input {
 public:
  virtual void encrypt(const std::string_view &crypto_name, const std::string &text_for_encoding, const char *key) = 0;

  virtual void decrypt(const std::string_view &crypto_name, const std::string &text_for_decoding, const char *key) = 0;
};

class CryptoInput : public Input {
 public:
  CryptoInput(DataView &data_view, CryptoStrategies &&crypto_strategies)
      : data_view { data_view }, crypto_strategies { std::move(crypto_strategies) } {}

  void encrypt(const std::string_view &crypto_strategy_name, const std::string &text_for_encoding,
               const char *key) override {
    try {
      try_encrypt(crypto_strategy_name, text_for_encoding, key);
    } catch (const std::exception &e) {
      data_view.output_text = e.what();
    }
  }

  void decrypt(const std::string_view &crypto_strategy_name, const std::string &text_for_decoding,
               const char *key) override {
    try {
      try_decrypt(crypto_strategy_name, text_for_decoding, key);
    } catch (const std::exception &e) {
      data_view.output_text = e.what();
    }
  }

 private:
  void try_encrypt(const std::string_view &crypto_strategy_name, const std::string &text_for_encoding,
                   const char *key) {
    auto &strategy { get_strategy(crypto_strategy_name) };
    if (strategy.is_key_numeric()) {
      encrypt_when_numeric_key(strategy, text_for_encoding, key);
    } else {
      encrypt_when_non_numeric_key(strategy, text_for_encoding, key);
    }
  }

  CryptoStrategy &get_strategy(const std::string_view &crypto_strategy_name) {
    return *crypto_strategies[crypto_strategy_name];
  }

  void encrypt_when_numeric_key(CryptoStrategy &strategy, const std::string &text_for_encoding, const char *key) {
    data_view.output_text = strategy.encrypt(text_for_encoding, std::stoi(key));
  }

  void encrypt_when_non_numeric_key(CryptoStrategy &strategy, const std::string &text_for_encoding, const char *key) {
    data_view.output_text = strategy.encrypt(text_for_encoding, key);
  }

  void try_decrypt(const std::string_view &crypto_strategy_name, const std::string &text_for_decoding,
                   const char *key) {
    auto &strategy { get_strategy(crypto_strategy_name) };
    if (strategy.is_key_numeric()) {
      decrypt_when_numeric_key(strategy, text_for_decoding, key);
    } else {
      decrypt_when_non_numeric_key(strategy, text_for_decoding, key);
    }
  }

  void decrypt_when_numeric_key(CryptoStrategy &strategy, const std::string &text_for_decoding, const char *key) {
    data_view.output_text = strategy.decrypt(text_for_decoding, std::stoi(key));
  }

  void decrypt_when_non_numeric_key(CryptoStrategy &strategy, const std::string &text_for_decoding, const char *key) {
    data_view.output_text = strategy.decrypt(text_for_decoding, key);
  }

  DataView &data_view;
  CryptoStrategies crypto_strategies;
};

#endif