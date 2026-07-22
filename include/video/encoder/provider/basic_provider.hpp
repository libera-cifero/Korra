#pragma once
#include "provider.hpp"
#include "len_reader/data_length_reader.hpp"
#include "video/encoder/provider/basic_block/basic_block_container.hpp"
#include "video/encoder/provider/basic_block/basic_block_settings.hpp"

class basic_provider : public provider {
private:
    basic_block_settings *_settings;
    basic_block_container *_container;
    uint8_t *_frame;
public:
    basic_provider(basic_block_settings *settings, data_length_reader *length_reader);
    char* to_data(vector<video_frame>*) override;
    vector<video_frame> *to_frames(char*) override;
    ~basic_provider();
};