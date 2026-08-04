#include <cstdint>
#include <string>
using namespace std;

uint8_t get_r(uint32_t color);
uint8_t get_g(uint32_t color);
uint8_t get_b(uint32_t color);

string rgb_to_hex(int rgb);