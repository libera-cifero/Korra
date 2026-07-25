#include "video/encoder/provider/mosaic_provider.hpp"
#include "video/encoder/provider/basic_block/basic_block_math.hpp"
#include "color.hpp"
#include "video/encoder/provider/basic_block/rgb_index.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

mosaic_provider::mosaic_provider(basic_block_settings *settings){
    _settings = settings;
    _block_count = get_block_count(settings->frame_width, settings->frame_height, settings->block_size);
    _payload_size = get_basic_block_frame_payload_size(settings->codec->bits_per_number(), _block_count);
    _frame_size = settings->frame_width * settings->frame_height * 3;
    _bits_per_block = _settings->codec->bits_per_number();
}

size_t mosaic_provider::frame_size() { return _frame_size; }

size_t mosaic_provider::payload_size() { return _payload_size; }

int mosaic_provider::_read_block_from_frame(char *frame, int block_index){
    point p0, p1, p;
    _block_index_to_area(block_index, p0, p1);

    uint32_t r_sum = 0, g_sum = 0, b_sum = 0, c = _settings->block_size * _settings->block_size;
    int frame_width = _settings->frame_width;

    rgb_index index;
    uint8_t *uframe = reinterpret_cast<uint8_t*>(frame);
    for(int y = p0.y; y < p1.y; y++) 
    {
        int row_base = 3 * (y * frame_width + p0.x);
        for(int x = p0.x; x < p1.x; x++, row_base += 3) {
            b_sum += uframe[row_base];
            g_sum += uframe[row_base + 1];
            r_sum += uframe[row_base + 2];
        }
    }

    c = ((r_sum / c) << 16) | ((g_sum / c) << 8) | (b_sum / c);
    return _settings->codec->color_to_number(c);
}

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
    char *payload = (char*)calloc(_frame_size, 1);
    bit_area area = {0, _bits_per_block};
    for(int i = 0; i < _block_count; i++){
        int block = _read_block_from_frame(frame, i);
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

void mosaic_provider::_block_index_to_area(int block_index, point &begin, point &end){
    int block_size = _settings -> block_size;
    int width_capacity = _settings -> frame_width / block_size;
    int index_y = block_index / width_capacity, index_x = block_index % width_capacity;
    uint32_t y = index_y * block_size, x = index_x * block_size;

    begin.x = x;
    begin.y = y;

    end.x = x + _settings->block_size;
    end.y = y + _settings->block_size;
}

void mosaic_provider::_draw_block(int block_index, int block_data, char *frame) {
    int color = _settings->codec->number_to_color(block_data);
    char red = get_r(color), green = get_g(color), blue = get_b(color);
    point p0, p1;
    _block_index_to_area(block_index, p0, p1);

    for(uint32_t y = p0.y; y < p1.y; y++) {
        for(uint32_t x = p0.x; x < p1.x; x++) {
            int b_pos = 3 * (y * _settings -> frame_width + x);
            int g_pos = b_pos + 1, r_pos = b_pos + 2;

            frame[r_pos] = red;
            frame[g_pos] = green;
            frame[b_pos] = blue;
        }
    }
}

char* mosaic_provider::to_frame(char* data) {
    char *frame = new char[_frame_size];
    memset(frame, 0, _frame_size);
    bit_area area = {0, _bits_per_block};
    for(int i = 0; i < _block_count; i++) {
        int block = _get_block(data, i);
        _draw_block(i, block, frame);
    }

    return frame;
}

mosaic_provider::~mosaic_provider(){
    delete _settings->codec;
    delete _settings;
}