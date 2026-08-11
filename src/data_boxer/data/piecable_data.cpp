#include "data_boxer/data/piecable_data.hpp"
#include <cstring>
#include <stdexcept>

void piecable_data::set_id(int id){
    __id = id;
}

void piecable_data::init_buffer(int buffer_size){
    __buffer_size = buffer_size;
    _writen = 0;
    _is_built = false;
    __buffer = new char[__buffer_size];
    memset(__buffer, 0, __buffer_size);
    _is_inited = true;
}

int piecable_data::size(){
    if(!_is_inited) throw std::runtime_error("piecable_data is not inited! Execute init_buffer previously!");
    return __buffer_size;
}

bool piecable_data::is_built(){
    return _is_built;
}

void piecable_data::put_piece(char *piece, int piece_size){
    if(!_is_inited) throw std::runtime_error("piecable_data is not inited! Execute init_buffer previously!");
    int delta = __buffer_size - _writen;
    int write_count = piece_size < delta ? piece_size : delta;
    memcpy(__buffer + _writen, piece, write_count);
    _writen += write_count;
    _is_built = delta <= piece_size;
}