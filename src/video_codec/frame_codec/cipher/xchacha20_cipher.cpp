#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cstring>
#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"

xchacha20_cipher::xchacha20_cipher(xchacha20_config *config) : cipher(config) {    
    _encryptor.SetKeyWithIV(config->key, KEY_SIZE, config->iv);
    _decryptor.SetKeyWithIV(config->key, KEY_SIZE, config->iv);
}

int xchacha20_cipher::header_size() { 
    return NONCE_SIZE + TAG_SIZE;
}

char *xchacha20_cipher::encrypt(char *data){
    byte nonce[NONCE_SIZE];
    _seed_gen.GenerateBlock(nonce, NONCE_SIZE);
    auto config = __config<xchacha20_config>();
    _encryptor.SetKeyWithIV(config->key, KEY_SIZE, nonce, NONCE_SIZE);

    int enc_size = encrypted_size();
    byte *encrypted = new byte[enc_size];

    memcpy(encrypted, nonce, NONCE_SIZE);
    ArraySource src (
        data, payload_size(),
        new AuthenticatedEncryptionFilter(
            _encryptor, 
            new ArraySink(
                encrypted + NONCE_SIZE, 
                enc_size - NONCE_SIZE
            ), 
            false,
            -1, 
            DEFAULT_CHANNEL
        )
    );
    
    return reinterpret_cast<char*>(encrypted);
}

char *xchacha20_cipher::decrypt(char *encrypted){
    const byte *data = reinterpret_cast<byte*>(encrypted);
    const byte* nonce = data;
    auto config = __config<xchacha20_config>();
    _decryptor.SetKeyWithIV(config -> key, KEY_SIZE, nonce, NONCE_SIZE);

    const byte* ciphertext = data + NONCE_SIZE;
    int size = payload_size();
    byte* decrypted = new byte[size];
    size_t tagged_data_size = size + TAG_SIZE;

    ArraySource src (
        ciphertext,
        tagged_data_size,
        true,
        new AuthenticatedDecryptionFilter(
            _decryptor, 
            new ArraySink(decrypted, size)
        )
    );
    
    return reinterpret_cast<char*>(decrypted);
}

xchacha20_cipher::~xchacha20_cipher(){
    auto config = __config<xchacha20_config>();

    delete [] config -> key;
    delete [] config -> iv;

    delete config;
}