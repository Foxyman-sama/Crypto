#ifndef AES_CRYPTO_HPP
#define AES_CRYPTO_HPP

#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>

#include "crypto_strategy.hpp"

class Utility {
 public:
  static const CryptoPP::byte *cast_to_byte(const std::string &text) {
    return reinterpret_cast<const CryptoPP::byte *>(&text[0]);
  }
};

class AESImplementation {
 public:
  virtual std::string encrypt(const std::string &text_for_encoding, const std::string &key) = 0;

  virtual std::string decrypt(const std::string &text_for_encoding, const std::string &key) = 0;
};

class CryptoLibAESImplementation : public AESImplementation {
 public:
  std::string encrypt(const std::string &text_for_encoding, const std::string &key) override {
    encryptor.SetKey(Utility::cast_to_byte(key), key.size());

    const auto encoded { encrypt_string(text_for_encoding) };
    return encode_in_hex(encoded);
  }

  std::string decrypt(const std::string &text_for_decoding, const std::string &key) override {
    decryptor.SetKey(Utility::cast_to_byte(key), key.size());

    const auto decoded_from_hex { decode_from_hex(text_for_decoding) };
    return decrypt_string(decoded_from_hex);
  }

 private:
  std::string encrypt_string(const std::string &text_for_encoding) {
    std::string result;
    const auto filter { new CryptoPP::StreamTransformationFilter { encryptor, new CryptoPP::StringSink { result } } };
    const CryptoPP::StringSource s { text_for_encoding, true, filter };
    return result;
  }

  std::string encode_in_hex(const std::string &decoded) {
    std::string result;
    CryptoPP::HexEncoder encoder { new CryptoPP::StringSink { result } };
    encoder.Put(Utility::cast_to_byte(decoded), decoded.size());
    encoder.MessageEnd();
    return result;
  }

  std::string decode_from_hex(const std::string &encoded) {
    std::string result;
    CryptoPP::HexDecoder decoder { new CryptoPP::StringSink { result } };
    decoder.Put(Utility::cast_to_byte(encoded), encoded.size());
    decoder.MessageEnd();
    return result;
  }

  std::string decrypt_string(const std::string &text_for_decoding) {
    std::string result;
    const auto filter { new CryptoPP::StreamTransformationFilter { decryptor, new CryptoPP::StringSink { result } } };
    const CryptoPP::StringSource s { text_for_decoding, true, filter };
    return result;
  }

  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryptor;
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryptor;
};

class AESCryptoStrategy : public CryptoStrategy {
 private:
  std::shared_ptr<AESImplementation> impl;

 public:
  AESCryptoStrategy(AESImplementation *impl = new CryptoLibAESImplementation) : impl { impl } {}

  std::string encrypt(const std::string &text_for_encoding, const std::any &any) override {
    return impl->encrypt(text_for_encoding, std::any_cast<const char *>(any));
  }

  std::string decrypt(const std::string &text_for_decoding, const std::any &any) override {
    return impl->decrypt(text_for_decoding, std::any_cast<const char *>(any));
  }

  bool is_key_numeric() noexcept override { return false; }
};
#endif