#include <cstdint>

class basic_block_container {
private:
    uint8_t *_frame;
    uint32_t _bits_per_block;
    uint32_t _block_size;

    uint32_t _frame_width;
    uint32_t _frame_height;

    uint32_t _block_count;
    uint32_t _block_capacity;

    uint32_t _width_capacity;
    uint32_t _height_capacity;

    struct rect{
        int x0;
        int x1;
        int y0; 
        int y1;
    };

    struct rgb_index{
        int r_index;
        int g_index;
        int b_index;
    };

    rect _get_rect_by_index(uint32_t block_index);
    rgb_index _get_index_by_point(int x, int y);
public:
    basic_block_container(uint8_t *frame, uint32_t bits_per_block, uint32_t block_size, uint32_t frame_width, uint32_t frame_height);
    uint32_t count();
    uint32_t capacity();
    uint32_t operator[](int block_index);
    void append_block(uint32_t data);
};