#include "video_codec/frame_codec/cipher/none_cipher.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include <cstring>

none_cipher::none_cipher(int payload_size) : cipher(payload_size) { }
char *none_cipher::encrypt(char *data) {
    int size = payload_size();
    char *encryptd = new char[size];
    memcpy(encryptd, data, size);
    return encryptd;
}

char *none_cipher::decrypt(char *encrypted){
    int size = payload_size();
    char *data = new char[size];
    memcpy(data, encrypted, size);
    return data;
}