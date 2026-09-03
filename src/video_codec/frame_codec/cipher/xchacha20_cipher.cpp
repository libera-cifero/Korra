#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <string>
#include <cstring>

xchacha20_cipher::xchacha20_cipher(xchacha20_settings &config) : cipher(config) {
    auto prefix = get_method_prefix("xchacha20_cipher.xchacha20_cipher");
    spdlog::debug("{} constructing...", prefix);
    _key = config.key;
    _iv = config.iv;
    spdlog::debug("{} _key and _iv ARE SECURE INFORMATION! PLEASE DON'T SHARE IT!!!\n_key={} _iv={}", prefix, get_byte_str(_key), get_byte_str(_iv));
    spdlog::info("{} constructing completed!", prefix);
}

int xchacha20_cipher::header_size() { 
    return NONCE_SIZE + TAG_SIZE;
}

crypto_byte *xchacha20_cipher::key(){
    return _key;
}

crypto_byte *xchacha20_cipher::iv(){
    return _iv;
}

char *xchacha20_cipher::encrypt(char *data){
    std::string prefix = get_method_prefix("xchacha20_cipher.encrypt"); 
    spdlog::info("{} encrypting...", prefix);

    crypto_byte* plaintext = reinterpret_cast<crypto_byte*>(data);

    int plaintext_len = payload_size();
    int ciphertext_len = plaintext_len + TAG_SIZE;
    int enc_len = NONCE_SIZE + ciphertext_len;
    if(is_debug_level())
        spdlog::debug("{} size={} data={}", prefix, plaintext_len, get_byte_str(data));

    crypto_byte* encrypted = new crypto_byte[enc_len];
    memset(encrypted, 0, enc_len);
    _seed_gen.GenerateBlock(encrypted, NONCE_SIZE);
    _encryptor.SetKeyWithIV(_key, KEY_SIZE, encrypted, NONCE_SIZE);

    CryptoPP::ArraySource src(
        plaintext,
        plaintext_len,
        true,
        new CryptoPP::AuthenticatedEncryptionFilter(
            _encryptor,
            new CryptoPP::ArraySink(
                encrypted + NONCE_SIZE,
                ciphertext_len
            ),
            false,
            -1,
            CryptoPP::DEFAULT_CHANNEL
        )
    );
    if(is_debug_level())
        spdlog::debug("{} size={} encrypted={}", prefix, enc_len, get_byte_str(data));
    spdlog::info("{} success!", prefix);

    return reinterpret_cast<char*>(encrypted);
}

char *xchacha20_cipher::decrypt(char *encrypted){
    auto prefix = get_method_prefix("xchacha20_cipher.decrypt");
    spdlog::info("{} decrypting...", prefix);
    int size = payload_size();
    size_t tagged_data_size = size + TAG_SIZE;
    if(is_debug_level())
        spdlog::debug("{} size={} encrypted={}", prefix, tagged_data_size, get_byte_str(encrypted));

    const crypto_byte *data = reinterpret_cast<crypto_byte*>(encrypted);
    const crypto_byte* nonce = data;
    _decryptor.SetKeyWithIV(_key, KEY_SIZE, nonce, NONCE_SIZE);

    const crypto_byte* ciphertext = data + NONCE_SIZE;
    crypto_byte* decrypted = new crypto_byte[size];

    CryptoPP::ArraySource src (
        ciphertext,
        tagged_data_size,
        true,
        new CryptoPP::AuthenticatedDecryptionFilter(
            _decryptor, 
            new CryptoPP::ArraySink(decrypted, size)
        )
    );
    if(is_debug_level())
        spdlog::debug("{} size={} data={}", prefix, size, get_byte_str(decrypted));
    spdlog::info("{} decryption success!", prefix);    
    return reinterpret_cast<char*>(decrypted);
}

xchacha20_cipher::~xchacha20_cipher(){
    auto prefix = get_method_prefix("xchacha20_cipher.~xchacha20_cipher");
    spdlog::debug(prefix);
    delete [] _key;
    delete [] _iv;
    spdlog::debug("{} destructing completed!", prefix);
}