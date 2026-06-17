#include <stdint.h>
#define COLOR_PALLETE 1536

#ifdef __cplusplus
extern "C"{
#endif
uint8_t get_r(uint32_t color);
uint8_t get_g(uint32_t color);
uint8_t get_b(uint32_t color);

uint32_t get_color_by_index(uint32_t color_index);
uint32_t get_index_by_color(uint32_t color);
#ifdef __cplusplus
}
#endif