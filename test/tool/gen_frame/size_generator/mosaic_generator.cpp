#include "size_generator/mosaic_generator.hpp"
#include "size_generator/size_generator.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include <numeric>
#include <random>

bool mosaic_generator::can_generate(provider *frame_provider) {
    return dynamic_cast<mosaic_provider*>(frame_provider) != nullptr;
}

area_size mosaic_generator::random_size(provider *frame_provider, area_size size_init) {
    int width_init = size_init.width, height_init = size_init.height;
    if(width_init > 0 && height_init > 0) return size_init;

    mosaic_provider *mosaic = (mosaic_provider*)frame_provider;
    mosaic_settings *settings = mosaic->settings();
    int lcm = std::lcm(8, settings->codec->bits_per_number()); 
    auto block_size = settings->block_size;
    int max_w_blocks = 1920 / block_size;
    int max_h_blocks = 1080 / block_size;
    
    int w_blocks, h_blocks;
    std::random_device seed; 
    std::mt19937_64 gen(seed());
    std::uniform_int_distribution<> dis_w(1, max_w_blocks);
    std::uniform_int_distribution<> dis_h(1, max_h_blocks);

    do {
        w_blocks = width_init <= 0 ? dis_w(gen): width_init;
        h_blocks = height_init <= 0 ? dis_h(gen): height_init;
    } while ((w_blocks * h_blocks) % lcm != 0);

    if(width_init <= 0) size_init.width = w_blocks * block_size;
    if(height_init <= 0) size_init.height = h_blocks * block_size;

    return size_init;
}
