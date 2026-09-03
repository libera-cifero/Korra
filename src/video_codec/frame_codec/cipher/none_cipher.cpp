#include "video_codec/frame_codec/cipher/none_cipher.hpp"
#include "config/data/cipher_config.hpp"
#include "video_codec/frame_codec/cipher/cipher.hpp"
#include "lib/log.hpp"
#include <spdlog/spdlog.h>
#include <cstring>
#include <string>

none_cipher::none_cipher(cipher_config &config) : cipher(config) { }

int none_cipher::header_size() { return 0; }

char *none_cipher::encrypt(char *data) {
    std::string prefix = get_method_prefix("none_cipher.encrypt"); 
    spdlog::info("{} encrypting...", prefix);
    int size = payload_size();
    if(is_debug_level()){
        spdlog::debug("{} size={} data={}", prefix, size, get_byte_str(data));
    }
    char *encrypted = new char[size];
    memcpy(encrypted, data, size);

    
    spdlog::debug("{} size={} encrypted={}", prefix, size, get_byte_str(encrypted));
    
    spdlog::info("{} success!", prefix);
    return encrypted;
}

char *none_cipher::decrypt(char *encrypted){
    std::string prefix = get_method_prefix("none_cipher.decrypt"); 
    spdlog::info("{} decrypting...", prefix);
    int size = payload_size();
    spdlog::debug("{} size={} encrypted={}", prefix, size, get_byte_str(encrypted));
    char *data = new char[size];
    memcpy(data, encrypted, size);
    spdlog::debug("{} size={} data={}", prefix, size, get_byte_str(data));
    spdlog::info("{} success!", prefix);
    return data;
}