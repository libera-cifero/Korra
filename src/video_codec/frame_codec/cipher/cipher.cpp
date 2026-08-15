#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "config/data/cipher_config.hpp"

cipher::cipher(cipher_config *config) {
    _config = config;
}
int cipher::encrypted_size(){ return _config -> encrypted_size; }
int cipher::payload_size(){ return _config -> encrypted_size - header_size(); }
cipher::~cipher() { delete _config; }