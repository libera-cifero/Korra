#pragma once
#include "provider.hpp"
#include "video/encoder/provider/basic_block/basic_block_settings.hpp"
#include "video/encoder/provider/basic_block/bit_area.hpp"
#include "video/encoder/provider/basic_block/point.hpp"
#include <cstddef>
#include <cstdint>

class mosaic_provider : public provider {
private:
    basic_block_settings *_settings;
    size_t _payload_size;
    size_t _frame_size;
    uint32_t _bits_per_block;
    uint32_t _block_count;


    void _block_index_to_area(int block_index, point &begins, point &end);

    void _draw_block(int block_index, int block_data, char *frame);
    int _get_block(char *data, int block_index);

    int _read_block_from_frame(char *frame, int block_index);
    bit_area _write_block_to_out(char *bytes, bit_area area, int block);

public:
    mosaic_provider(basic_block_settings *settings);
    size_t frame_size() override;
    size_t payload_size() override;

    char* to_payload(char* frame) override;
    char* to_frame(char* data) override;
    ~mosaic_provider();
};