#pragma once
#include "provider.hpp"
#include "len_reader/data_length_reader.hpp"
struct basic_provider_config {
    int window_width;
    int window_height;
    int block_size;
    int bit_color_resolution;
    int fps_payload;
    int fps_video;

    data_length_reader *reader;
};

class basic_provider : public provider {
private:
    basic_provider_config _config;
public:
    basic_provider(basic_provider_config config);
    char *pull_data(char **frames, int *date_count) override;
    char **push_frames(char *data, int *frame_count) override;
    virtual ~basic_provider();
};