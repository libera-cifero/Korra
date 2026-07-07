#include <cstdint>
#include <string>
using namespace std;
#define COLOR_palette 1530

uint8_t get_r(uint32_t color);
uint8_t get_g(uint32_t color);
uint8_t get_b(uint32_t color);

uint32_t get_color_by_index(uint32_t color_index);
uint32_t get_index_by_color(uint32_t color);

uint32_t number_to_index(uint32_t number, int color_bit_resolution);
uint32_t index_to_number(uint32_t number, int color_bit_resolution);

uint32_t number_to_color(uint16_t number, int color_bit_resolution);
uint16_t color_to_number(uint32_t color, int color_bit_resolution);

string rgb_to_hex(int rgb);