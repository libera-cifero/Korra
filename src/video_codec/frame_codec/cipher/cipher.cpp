#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "config/data/cipher_config.hpp"
#include <concepts>

cipher::cipher(cipher_config *config) {
    _config = config;
}
int cipher::encrypted_size(){ return _config -> encrypted_size; }
int cipher::payload_size(){ return _config -> encrypted_size - header_size(); }
template<std::derived_from<cipher_config> T>
T *cipher::__config(){ return static_cast<T*>(_config); }
cipher::~cipher() { delete _config; }