#include "video_codec/frame_codec/cipher/cipher.hpp"

cipher::cipher(int payload_size) {
    _payload_size = payload_size;
}

int cipher::payload_size(){ return _payload_size; }
cipher::~cipher() { }