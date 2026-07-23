#pragma once
#include "provider.hpp"
#include "video/encoder/provider/basic_block/basic_block_container.hpp"
#include "video/encoder/provider/basic_block/basic_block_settings.hpp"
#include <cstddef>

class basic_provider : public provider {
private:
    basic_block_settings *_settings;
    basic_block_container *_container;
    uint8_t *_frame;
    size_t _payload_size;
    size_t _frame_size;
public:
    basic_provider(basic_block_settings *settings);
    size_t frame_size() override;
    size_t payload_size() override;

    char* to_payload(char* frame) override;
    char* to_frame(char* data) override;
    ~basic_provider();
};