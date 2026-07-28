#include "data_boxer/checker/ip_data_checker.hpp"
#include <cstdint>

BYTE_ITERATOR
bool ip_data_checker<iterator_type>::is_size_readable(iterator_type data_begin, iterator_type frame_end){
    return frame_end - data_begin >= 4;
}

BYTE_ITERATOR
bool ip_data_checker<iterator_type>::is_data_readable(uint32_t size, iterator_type data_begin, iterator_type frame_end){
    return frame_end - data_begin >= 2 + size;
}