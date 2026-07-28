#pragma once
#include "data_checker.hpp"
#include <cstdint>

BYTE_ITERATOR
class ip_data_checker : public data_checker<iterator_type> {
public:
    bool is_size_readable(iterator_type data_begin, iterator_type frame_end) override;
    bool is_data_readable(uint32_t size, iterator_type data_begin, iterator_type frame_end) override;

    ~ip_data_checker();
};