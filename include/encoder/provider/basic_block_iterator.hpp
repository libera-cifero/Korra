#include <cstddef>
#include <cstdint>
#include <iterator>

class basic_block_container {
private:
    char *_frame;
    uint32_t _bits_per_block;
    uint32_t _block_size;

    uint32_t _frame_width;
    uint32_t _frame_height;

    uint32_t _block_index;
public:
    class basic_block_iterator 
    {
    private:
        uint32_t _parse_block_data();
        uint32_t _append_block_data();
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = uint32_t;
        using pointer = uint32_t*;
        using reference = uint32_t&;

        reference operator*() const;
        pointer operator->();
    };
};