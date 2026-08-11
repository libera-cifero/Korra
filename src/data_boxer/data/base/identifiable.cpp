#include "data_boxer/data/base/identifiable.hpp"
#include <cstdint>
#include <ctime>
#include <random>

identifiable::identifiable() { }

identifiable::identifiable(int id, uint16_t data_size){
    __id = id;
    __data_size = data_size;
}

int identifiable::_generate_random_id(){
    std::random_device dev;
    std::seed_seq seq{
        dev(),   
        static_cast<uint32_t>(std::time(nullptr)),
        dev()
    };
    std::mt19937 random(seq);
    return random();
}

identifiable::identifiable(uint16_t data_size){
    __id = _generate_random_id();
    __data_size = data_size;
}

int identifiable::id() { return __id; }

uint16_t identifiable::data_size() { return __data_size; }