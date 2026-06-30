#include "math.h"

static size_t get_gcd(size_t a, size_t b) {
    if (b == 0)
		return a;
	return get_gcd(b, a % b);
}

size_t get_lcm(size_t a, size_t b, size_t min_res){
    size_t y = a * b / get_gcd(a, b);
    size_t n = min_res / y;
    if(min_res % y > 0) n += 1;
    return n * y;
}