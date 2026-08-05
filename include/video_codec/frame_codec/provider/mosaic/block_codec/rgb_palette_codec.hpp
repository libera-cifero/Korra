#pragma once
#include "palette_codec.hpp"

class rgb_palette_codec : public palette_codec<int> {
private:
    int _find_nearest(char r, char g, char b);
public:
    rgb_palette_codec(palette_codec_config<int> &config);

    void encode(char *frame, int number, int block_index) override;
    int decode(char *frame, int block_index) override;
};