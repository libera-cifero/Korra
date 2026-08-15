#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "config/data/cipher_config.hpp"

cipher::cipher(cipher_config &config) {
    __config = config;
}
int cipher::encrypted_size(){ return __config.encrypted_size; }
int cipher::payload_size(){ return __config.encrypted_size - header_size(); }

cipher::~cipher() { }