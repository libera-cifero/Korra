#include "data_boxer/data/base/identifiable.hpp"
#include <cstdint>
#include <ctime>
#include <random>

identifiable::identifiable() { }

identifiable::identifiable(int id, uint16_t data_size){
    $id = id;
    $data_size = data_size;
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
    $id = _generate_random_id();
    $data_size = data_size;
}

int identifiable::id() { return $id; }

uint16_t identifiable::data_size() { return $data_size; }