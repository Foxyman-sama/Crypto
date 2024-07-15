#ifndef CRYPTO_STRATEGY
#define CRYPTO_STRATEGY

#include <any>
#include <string>

class CryptoStrategy {
 public:
  virtual std::string encrypt(const std::string &text_for_encoding, const std::any &any) = 0;

  virtual std::string decrypt(const std::string &text_for_decoding, const std::any &anyy) = 0;

  virtual bool is_key_numeric() noexcept = 0;
};

#endif