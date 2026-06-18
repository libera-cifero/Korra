#include <bitset>
#include <cstdint>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/encoder/video_encoder.hpp"
#include "../include/color.h"


video_encoder::video_encoder(video_encoder_config config){
    _config = config;
    
    int width_count = _config.window_width / _config.block_size;
    int height_count = _config.window_height / _config.block_size;
    _blocks_per_frame = width_count * height_count;
    _blocks_per_package = IP_PACKAGE_BIT_SIZE / _config.bit_color_resolution;
    _blocks_per_package += IP_PACKAGE_BIT_SIZE % _config.bit_color_resolution > 0 ? 1 : 0;

    _blocks_per_last_frame = _blocks_per_package % _blocks_per_frame;

    _frames_per_package = _blocks_per_package / _blocks_per_frame + (_blocks_per_last_frame > 0 ? 1 : 0);
    _blocks_per_row = width_count;
}

int video_encoder::frames_per_package(){
    return _frames_per_package;
}

block video_encoder::_get_block_info(int block_index){
    int ix = block_index % _blocks_per_row, iy = block_index / _blocks_per_row;
    int x0 = ix * _config.block_size, y0 = iy * _config.block_size;
    int x1 = x0 + _config.block_size, y1 = y0 + _config.block_size;

    return {.x0 = x0, .x1 = x1, .y0 = y0, .y1 = y1};
}

color_index video_encoder::_get_color_index(int x, int y){
    int ir = (y * _config.window_width + x) * 3;
    int ig = ir + 1;
    int ib = ir + 2;

    return { .red_index = ir, .green_index = ig, .blue_index = ib };
}

void video_encoder::_print_block(char *frame, uint32_t block_index, uint32_t color){
    uint8_t r = get_r(color), g = get_g(color), b = get_b(color);

    block info = _get_block_info(block_index);

    for(int y = info.y0; y < info.y1; y++){
        for(int x = info.x0; x < info.x1; x++){
            color_index index = _get_color_index(x, y);

            frame[index.red_index] = (char)r;
            frame[index.green_index] = (char)g;
            frame[index.blue_index] = (char)b;
        }
    }
}

char* video_encoder::_encode_to_frame(bitset<IP_PACKAGE_BIT_SIZE> &ip_package, int bits_read, int blocks_to_encode) {
    int bit_end = bits_read + blocks_to_encode * _config.bit_color_resolution;
    char *frame = (char*)calloc(3 * _config.window_width * _config.window_height, 1);
    uint32_t block_index = 0;

    for(int ip_bit_index = bits_read; ip_bit_index <= bit_end; ip_bit_index += _config.bit_color_resolution){
        uint32_t data = 0;
        for(int i = 0; i < _config.bit_color_resolution; i++){
            bool bit = ip_package[bits_read+i];
            data |= bit ? (1 << i) : 0;
        }
        uint32_t color_index = data * COLOR_PALLETE / (1 << _config.bit_color_resolution);
        uint32_t block_color = get_color_by_index(color_index);
        _print_block(frame,block_index++, block_color);
    }

    return frame;
}

char** video_encoder::encode_package(char *ip_package)
{
    char **frames = (char**)malloc(frames_per_package() * sizeof(char*));
    int bits_read = 0, last_frame = frames_per_package() - 1;

    bitset<IP_PACKAGE_BIT_SIZE> package_bits(ip_package);

    for(int frame = 0; frame <= last_frame; frame++){
        int blocks_per_frame = frame < last_frame ? _blocks_per_frame : _blocks_per_last_frame;
        frames[frame] = _encode_to_frame(package_bits, bits_read, blocks_per_frame);
        bits_read += _blocks_per_frame * _config.bit_color_resolution;
    }

    return frames;
}

uint32_t video_encoder::_get_block_color(char *frame, block info) 
{
    uint32_t color = 0, color_buf, red_sum = 0, green_sum = 0, blue_sum = 0;
    for(int y = info.y0; y < info.y1; y++) 
    {
        for(int x = info.x0; x < info.x1; x++) 
        {
            color_index index = _get_color_index(x, y);
            char red = frame[index.red_index], green = frame[index.green_index], blue = frame[index.blue_index];
            red_sum += ((uint32_t)red) << 16;
            green_sum += ((uint32_t)green) << 8;
            blue_sum += (uint32_t)blue;
        }
    }

    uint32_t quatient = (info.x1 - info.x0) * (info.y1 - info.y0);
    uint32_t red = red_sum / quatient, green = green_sum / quatient, blue = blue_sum / quatient;
    return (red << 16) | (green << 8) | blue;
}

void video_encoder::_decode_frame(char *ip_package, char *frame, int frame_index) 
{
    for(int block_index = 0; block_index < _blocks_per_frame; block_index++) 
    {
        block info = _get_block_info(block_index);
        uint32_t block_color = _get_block_color(frame, info);
        uint32_t data_piece = color_to_number(block_color, _config.bit_color_resolution);
        
        frame_index += _config.bit_color_resolution;
    }
}

char* video_encoder::decode_package(char **frames)
{
    char *ip_package = (char*)malloc(IP_PACKAGE_SIZE);
    for(int frame_index = 0; frame_index < _frames_per_package; frame_index++){
        _decode_frame(ip_package, frames[frame_index], frame_index);
    }

    return ip_package;
}