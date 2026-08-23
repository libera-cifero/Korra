#include <cstdint>
#include <cstring>
#include <filesystem>
#include <tuple>

char *random_array(uint64_t seed, size_t size, uint64_t &next_seed);
uint64_t get_seed(const std::filesystem::path &file_path);
std::tuple<char*, int> *get_random_payloads(const std::filesystem::path &seed_path, int *lengths, int len_count);
void free_random_payloads(std::tuple<char*, int> *payloads, int len_count);