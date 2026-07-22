#include "video/encoder/provider/provider.hpp"
#include "video/encoder/provider/len_reader/data_length_reader.hpp"

provider::provider(data_length_reader *len_reader){
    _length_reader = len_reader;
}

data_length_reader* provider::length_reader(){
    return _length_reader;
}

provider::~provider(){
    delete _length_reader;
}

