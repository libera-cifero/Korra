#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "config/data/cipher_config.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>

cipher::cipher(cipher_config &config) {
    auto prefix = get_method_prefix("cipher.cipher");
    spdlog::debug("{} constructing...");
    __config = config;
    spdlog::debug("{} constructing completed!");
}
int cipher::encrypted_size(){ return __config.encrypted_size; }
int cipher::payload_size(){ return __config.encrypted_size - header_size(); }

cipher::~cipher() { 
    auto prefix = get_method_prefix("cipher.cipher");
    spdlog::debug("{} destucted!");
}