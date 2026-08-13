#include "video_codec/frame_codec/cipher/xchacha20_cipher.hpp"

void test_encode_decode() {
    xchacha20_config *config = new xchacha20_config;
    config->key = new byte[xchacha20_cipher::KEY_SIZE];
    auto cipher = new xchacha20_cipher(config);
}

int main(){
    test_encode_decode();
    return 0;
}