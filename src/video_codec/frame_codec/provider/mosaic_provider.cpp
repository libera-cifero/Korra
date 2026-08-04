#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_math.hpp"
#include "lib/color.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

mosaic_provider::mosaic_provider(mosaic_settings *settings){
    _settings = settings;
    _block_count = get_block_count(settings->frame_width, settings->frame_height, settings->block_size);
    _payload_size = get_mosaic_frame_payload_size(settings->codec->bits_per_number(), _block_count);
    _frame_size = settings->frame_width * settings->frame_height * 3;
    _bits_per_block = _settings->codec->bits_per_number();
}

size_t mosaic_provider::frame_size() { return _frame_size; }

size_t mosaic_provider::payload_size() { return _payload_size; }

bit_area mosaic_provider::_write_block_to_out(char *out, bit_area area, int block) {
    //bd - bit data, bb - bit block
    uint32_t bit0 = area.bit0, bit1 = area.bit1;
    for(size_t bd = bit0; bd < bit1; bd++){ 
        size_t bb = bd-bit0;
        char bit = (char)((block >> bb) & 1);

        size_t byte_index = bd / 8;
        size_t bit_index = bd % 8;
        char byte = out[byte_index];
        byte |= bit << bit_index;
        out[byte_index] = byte;
    }
    area.bit0 = bit1;
    area.bit1 += _bits_per_block;
    return area;
}

char* mosaic_provider::to_payload(char* frame) {
    char *payload = new char[_block_count];
    memset(payload, 0, _block_count);
    bit_area area = {0, _bits_per_block};
    for(int i = 0; i < _block_count; i++){
        int block = _settings->codec->decode(frame, i);
        area = _write_block_to_out(payload, area, block);
    }

    return payload;
}

int mosaic_provider::_get_block(char *data, int block_index) {
    int b0 = _bits_per_block * block_index;
    int b1 = b0 + _bits_per_block;
    int block = 0;
    for(int b = b0; b < b1; b++) {
        int byte_index = b / 8;
        int bit_index = b % 8;

        int bit = (data[byte_index] >> bit_index) & 1;
        block |= bit << (b - b0);
    }

    return block;
}

char* mosaic_provider::to_frame(char* data) {
    char *frame = new char[_frame_size];
    memset(frame, 0, _frame_size);
    bit_area area = {0, _bits_per_block};
    for(int i = 0; i < _block_count; i++) {
        int block = _get_block(data, i);
        _settings->codec->encode(frame, block, i);
    }

    return frame;
}

mosaic_provider::~mosaic_provider(){
    delete _settings->codec;
    delete _settings;
}