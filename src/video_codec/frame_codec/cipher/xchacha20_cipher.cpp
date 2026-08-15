#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cstring>
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"


xchacha20_cipher::xchacha20_cipher(xchacha20_settings *config) : cipher(config) {    
    _encryptor.SetKeyWithIV(config->key, KEY_SIZE, config->iv);
    _decryptor.SetKeyWithIV(config->key, KEY_SIZE, config->iv);
}

int xchacha20_cipher::header_size() { 
    return NONCE_SIZE + TAG_SIZE;
}

char *xchacha20_cipher::encrypt(char *data){
    CryptoPP::byte nonce[NONCE_SIZE];
    _seed_gen.GenerateBlock(nonce, NONCE_SIZE);
    auto cfg = config<xchacha20_settings>();
    _encryptor.SetKeyWithIV(cfg->key, KEY_SIZE, nonce, NONCE_SIZE);

    int enc_size = encrypted_size();
    auto encrypted = new CryptoPP::byte[enc_size];

    memcpy(encrypted, nonce, NONCE_SIZE);
    CryptoPP::ArraySource src (
        data, payload_size(),
        new CryptoPP::AuthenticatedEncryptionFilter(
            _encryptor, 
            new CryptoPP::ArraySink(
                encrypted + NONCE_SIZE, 
                enc_size - NONCE_SIZE
            ), 
            false,
            -1, 
            CryptoPP::DEFAULT_CHANNEL
        )
    );
    
    return reinterpret_cast<char*>(encrypted);
}

char *xchacha20_cipher::decrypt(char *encrypted){
    const auto data = reinterpret_cast<CryptoPP::byte*>(encrypted);
    const auto nonce = data;
    auto cfg = config<xchacha20_settings>();
    _decryptor.SetKeyWithIV(cfg -> key, KEY_SIZE, nonce, NONCE_SIZE);

    const auto ciphertext = data + NONCE_SIZE;
    int size = payload_size();
    auto decrypted = new CryptoPP::byte[size];
    size_t tagged_data_size = size + TAG_SIZE;

    CryptoPP::ArraySource src (
        ciphertext,
        tagged_data_size,
        true,
        new CryptoPP::AuthenticatedDecryptionFilter(
            _decryptor, 
            new CryptoPP::ArraySink(decrypted, size)
        )
    );
    
    return reinterpret_cast<char*>(decrypted);
}