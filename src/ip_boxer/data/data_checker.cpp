#include "ip_boxer/data/data_checker.hpp"

BYTE_ITERATOR
bool data_checker<iterator_type>::is_type_readable(iterator_type data_begin, iterator_type frame_end){
    return frame_end - data_begin > 2;
}

BYTE_ITERATOR
data_checker<iterator_type>::~data_checker() { }