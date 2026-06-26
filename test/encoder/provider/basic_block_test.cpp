#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "color.h"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.h"
#include "test.h"
#include <cstdint>
#include <exception>
#include <tuple>

#define TEST_BASE_NAME "basic_block_test"
#define PTR_TEST_BASE_NAME TEST_BASE_NAME ".basic_block_pointer"
using namespace std;

//basic_block_pointer
//=====BEGIN=====
void test_ptr_equal(){
    const char *test_name = PTR_TEST_BASE_NAME ".test_ptr_equal";
    printInfo(test_name);
    basic_block_config config = {
        .bits_per_block = 6,
        .block_size = 4,
        .frame_width = 10,
        .frame_height = 12
    };
    const int frame_size = 3 * config.frame_width * config.frame_height;
    uint8_t frame[frame_size];

    basic_block_pointer ptr_nullptr0 = nullptr;
    basic_block_pointer ptr_nullptr1 = nullptr;

    basic_block_pointer ptr00(frame, 0, &config);
    basic_block_pointer ptr01(frame, 0, &config);

    basic_block_pointer ptr10(frame, 4, &config);
    basic_block_pointer ptr11(frame + 3, 0, &config);


    if(ptr_nullptr0 != ptr_nullptr1) {
        fail(test_name, "ptr_nullptr0 != ptr_nullptr1", 1);
    }
    if(ptr_nullptr1 != ptr_nullptr0){
        fail(test_name, "ptr_nullptr1 != ptr_nullptr0", 1);
    }
    
    if(ptr_nullptr0 == ptr00){
        fail(test_name, "ptr_nullptr1 == ptr_nullptr0", 1);
    }
    if(ptr_nullptr0 == ptr01){
        fail(test_name, "ptr_nullptr0 == ptr01", 1);
    }
    if(ptr_nullptr0 == ptr10){
        fail(test_name, "ptr_nullptr0 == ptr10", 1);
    }
    if(ptr_nullptr0 == ptr11){
        fail(test_name, "ptr_nullptr0 == ptr11", 1);
    }
    
    if(ptr00 != ptr01){
        fail(test_name, "ptr00 != ptr01", 1);
    }
    if(ptr01 != ptr00){
        fail(test_name, "ptr01 != ptr00", 1);
    }

    if(ptr10 != ptr11){
        fail(test_name, "ptr10 != ptr11", 1);
    }
    if(ptr11 != ptr10){
        fail(test_name, "ptr11 != ptr10", 1);
    }

    printPass(test_name);
}
//======END======

//basic_block_container
//=====BEGIN=====
void test_get_block_by_index() {
    const char *test_name = TEST_BASE_NAME ".test_get_block_by_index";
    printInfo(test_name);

    const uint32_t num0 = 34, num1 = 13, num2 = 2, num3 = 27;
    const uint32_t bits_per_block = 6;
    uint32_t colors[]={
        number_to_color(num0, bits_per_block),
        number_to_color(num1, bits_per_block),
        number_to_color(num2, bits_per_block),
        number_to_color(num3, bits_per_block)
    };

    const int count = sizeof(colors) / 4;
    uint8_t rgb[count][3];
    for(int i = 0; i < count; i++){
        uint32_t color = colors[i];
        uint32_t r = get_r(color), g = get_g(color), b = get_b(color);
        rgb[i][0] = r;
        rgb[i][1] = g;
        rgb[i][2] = b;
    }

    uint8_t r0 = rgb[0][0], g0 = rgb[0][1], b0 = rgb[0][2];
    uint8_t r1 = rgb[1][0], g1 = rgb[1][1], b1 = rgb[1][2];
    uint8_t r2 = rgb[2][0], g2 = rgb[2][1], b2 = rgb[2][2];
    uint8_t r3 = rgb[3][0], g3 = rgb[3][1], b3 = rgb[3][2];

    const int h = 10, w = 12;
    const int frame_size = 3*h*w;

    uint8_t frames[][frame_size]={
        {
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  r0,g0,b0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0
        },
        {
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,  r1,g1,b1,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0
        },
        {
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  r2,g2,b2,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0
        },
        {
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  r3,g3,b3,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,
            0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0,  0,0,0
        }
    };
    //uint32_t output, uint8_t *frame, uint32_t bits_per_block, uint32_t block_size, uint32_t frame_width, uint32_t frame_height
    tuple<uint32_t, int> test_cases[]={
        { num0, 1 },
        { num1, 2 },
        { num2, 4 },
        { num3, 3 }
    };

    const int frame_count = sizeof(frames) / frame_size / sizeof(uint8_t);
    basic_block_config config = {.bits_per_block = bits_per_block, .block_size = 4, .frame_width = w, .frame_height = h};
    for(int i = 0; i < frame_count; i++){
        try{
            basic_block_container container(frames[i], config);
            auto test = test_cases[i];
            uint32_t expected = get<0>(test);
            int index = get<1>(test);
            uint32_t out = container[index];
            if(expected != out){
                printFail(test_name, "frames[%d] container[%d] expected %u, but got %u",i, index, expected, out);
                return;
            }
        }
        catch (exception *e){
            printFail(test_name, e->what());
            return;
        }
    }

    printPass(test_name);
}

void test_set_block_by_index(){

}
//======END======

int main() {
    test_get_block_by_index();
    test_set_block_by_index();

    test_ptr_equal();
    return 0;
}