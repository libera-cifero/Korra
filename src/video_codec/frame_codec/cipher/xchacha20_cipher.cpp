#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cstring>
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"

xchacha20_cipher::xchacha20_cipher(xchacha20_settings &config) : cipher(config) {    
    _key = config.key;
    _iv = config.iv;
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
    crypto_byte* plaintext = reinterpret_cast<crypto_byte*>(data);

    int plaintext_len = payload_size();
    int ciphertext_len = plaintext_len + TAG_SIZE;
    int enc_len = NONCE_SIZE + ciphertext_len;

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

    return reinterpret_cast<char*>(encrypted);
}

char *xchacha20_cipher::decrypt(char *encrypted){
    const crypto_byte *data = reinterpret_cast<crypto_byte*>(encrypted);
    const crypto_byte* nonce = data;
    _decryptor.SetKeyWithIV(_key, KEY_SIZE, nonce, NONCE_SIZE);

    const crypto_byte* ciphertext = data + NONCE_SIZE;
    int size = payload_size();
    crypto_byte* decrypted = new crypto_byte[size];
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

xchacha20_cipher::~xchacha20_cipher(){
    delete [] _key;
    delete [] _iv;
}