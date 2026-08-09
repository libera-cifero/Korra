#include "data_boxer/data/piecable_data.hpp"
#include <cstring>
#include <stdexcept>

void piecable_data::set_id(int id){
    $id = id;
}

void piecable_data::init_buffer(int buffer_size){
    $buffer_size = buffer_size;
    _writen = 0;
    _is_built = false;
    $buffer = new char[$buffer_size];
    memset($buffer, 0, $buffer_size);
    _is_inited = true;
}

int piecable_data::size(){
    if(!_is_inited) throw std::runtime_error("piecable_data is not inited! Execute init_buffer previously!");
    return $buffer_size;
}

bool piecable_data::is_built(){
    return _is_built;
}

void piecable_data::put_piece(char *piece, int piece_size){
    if(!_is_inited) throw std::runtime_error("piecable_data is not inited! Execute init_buffer previously!");
    int delta = $buffer_size - _writen;
    int write_count = piece_size < delta ? piece_size : delta;
    memcpy($buffer + _writen, piece, write_count);
    _writen += delta;
    _is_built = delta <= piece_size;
}