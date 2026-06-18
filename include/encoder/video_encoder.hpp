#include "compressor/compressor.hpp"
#include "obfuscator/obfuscator.hpp"
#include "recoverer/recoverer.hpp"
#include "provider/provider.hpp"
#include <bitset>
#include <cstdint>
#define IP_PACKAGE_SIZE 65536
#define IP_PACKAGE_BIT_SIZE 524288

using namespace std;
typedef struct video_encoder_config {
    int window_width;
    int window_height;
    int block_size;
    int bit_color_resolution;
    int fps_payload;
    int fps_video;
} video_encoder_config;

typedef struct block{
    int x0;
    int x1;
    int y0;
    int y1;
} block;

typedef struct color_index 
{
    int red_index;
    int green_index;
    int blue_index;
} color_index;

class video_encoder {
private:
    video_encoder_config _config;
    obfuscator *_obfuscator;
    compressor *_compressor;
    recoverer *_recoverer;
    provider *_frame_manager;

    int _blocks_per_package;
    int _blocks_per_frame;
    int _blocks_per_last_frame;
    int _frames_per_package;
    int _blocks_per_row;

    block _get_block_info(int block_index);
    color_index _get_color_index(int x, int y);

    void _print_block(char *frame, uint32_t block_index, uint32_t color);
    char *_encode_to_frame(bitset<IP_PACKAGE_BIT_SIZE> &ip_package, int bits_read, int blocks_to_encode);

    uint32_t _get_block_color(char *frame, block info);
    uint16_t _get_ip_package_size(char **frames, int frame_count);
    void _decode_frame(char *ip_package, char *frame, int frame_index);
public:
    video_encoder(video_encoder_config config);
    char **encode_package(char *ip_package);
    char *decode_package(char **frames);

    int frames_per_package();
};