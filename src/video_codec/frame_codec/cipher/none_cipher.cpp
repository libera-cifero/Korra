#include "video_codec/frame_codec/cipher/none_cipher.hpp"
#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cstring>

none_cipher::none_cipher(cipher_config *config) : cipher(config) { }

int none_cipher::header_size() { return 0; }

char *none_cipher::encrypt(char *data) {
    int size = payload_size();
    char *encrypted = new char[size];
    memcpy(encrypted, data, size);
    return encrypted;
}

char *none_cipher::decrypt(char *encrypted){
    int size = payload_size();
    char *data = new char[size];
    memcpy(data, encrypted, size);
    return data;
}