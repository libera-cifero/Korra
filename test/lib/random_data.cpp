#include "random_data.hpp"
#include <fstream>

static inline uint64_t hash64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

char *random_array(uint64_t seed, size_t size, uint64_t &next_seed) {
    char *array = new char[size];
    
    uint64_t current_seed = seed;
    
    for (size_t i = 0; i < size; ++i) {
        uint64_t hash_input = current_seed ^ hash64((uint64_t)i);
        uint64_t hash_output = hash64(hash_input);
        
        array[i] = (char)(hash_output & 0xFF);
    }
    
    next_seed = hash64(current_seed ^ hash64(size));
    
    return array;
}

uint64_t get_seed(const std::filesystem::path &file_path) {
    std::ifstream file(file_path, std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("File can't be open " + file_path.string());
    }

    uint64_t seed = 0xcbf29ce484222325ULL; 
    const uint64_t FNV_prime = 0x100000001b3ULL;
    
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        size_t bytes_read = file.gcount();
        
        for (size_t i = 0; i < bytes_read; ++i) {
            seed ^= (uint8_t)buffer[i];
            seed *= FNV_prime;
            
            if ((i + 1) % 8 == 0) {
                seed = hash64(seed);
            }
        }
    }
    
    file.close();
    
    return hash64(seed);
}

std::tuple<char*, int> *get_random_payloads(const std::filesystem::path &seed_path, int *lengths, int len_count) {
    uint64_t current_seed = get_seed(seed_path);
    
    std::tuple<char*, int> *payloads = new std::tuple<char*, int>[len_count];
    
    for (int i = 0; i < len_count; ++i) {
        int size = lengths[i];
        
        char *array = random_array(current_seed, size, current_seed);
        
        payloads[i] = std::make_tuple(array, size);
    }
    
    return payloads;
}

void free_random_payloads(std::tuple<char*, int> *payloads, int len_count) {
    if (payloads == nullptr) {
        return;
    }
    
    for (int i = 0; i < len_count; ++i) {
        char *array = std::get<0>(payloads[i]);
        delete[] array;
    }
    
    delete[] payloads;
}