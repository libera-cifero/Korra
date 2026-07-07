#include "color.hpp"
#include "../include/status_error.hpp"
#include "test.h"
#include <cstdint>
#include <stdint.h>

void test_get_color_by_index(){
    const char *test_name = "color_test.test_get_color_by_index";
    printInfo(test_name);
    uint32_t test_cases[][2]={
        //control points
        {0,0xff0000},
        {255,0xffff00},
        {510,0x00ff00},
        {765,0x00ffff},
        {1020,0x0000ff},
        {1275,0xff00ff},
        {1529,0xff0001},

        //around control points
        {1,0xff0100},
        {254,0xfffe00},
        {509,0x01ff00},
        {764,0x00fffe},
        {1019,0x0001ff},
        {1274,0xfe00ff},
        {1528,0xff0002},
        {256,0xfeff00},
        {511,0x00ff01},
        {766,0x00feff},
        {1021,0x0100ff},
        {1276,0xff00fe},

        //random points
        {42,   0xFF2A00},
        {300,  0xD2FF00},
        {700,  0x00FFBE},
        {900,  0x0078FF},
        {1400, 0xFF0082},
        
        //exceptions
        {1530,UINT32_MAX},
        {19999, UINT32_MAX},
        {284894, UINT32_MAX}
    };

    int test_count = sizeof(test_cases)/2/sizeof(uint32_t);
    for(int i = 0; i < test_count; i++){
        uint32_t in = test_cases[i][0], out_predicted = test_cases[i][1];
        uint32_t out = get_color_by_index(in);
        if(out != out_predicted){
            printFail(test_name, "get_color_by_index(%u) must be equals %06x, but returns %06x", in, out_predicted, out);
            return;
        }
    }

    printPass(test_name);
}

void test_get_index_by_color(){
    const char *test_name = "color_test.test_get_index_by_color";
    printInfo(test_name);

    uint32_t test_cases[][2]={
        //control points
        {0xff0000, 0},
        {0xffff00, 255},
        {0x00ff00, 510},
        {0x00ffff, 765},
        {0x0000ff, 1020},
        {0xff00ff, 1275},
        {0xff0001, 1529},

        //around control points
        {0xff0100, 1},
        {0xfffe00, 254},
        {0x01ff00, 509},
        {0x00fffe, 764},
        {0x0001ff, 1019},
        {0xfe00ff, 1274},
        {0xff0002, 1528},
        {0xfeff00, 256},
        {0x00ff01, 511},
        {0x00feff, 766},
        {0x0100ff, 1021},
        {0xff00fe, 1276},

        //exceptions
        {0xffff0000, UINT32_MAX},
    };

    int test_count = sizeof(test_cases)/2/sizeof(uint32_t);
    for(int i = 0; i < test_count; i++){
        uint32_t in = test_cases[i][0], out_predicted = test_cases[i][1];
        uint32_t out = get_index_by_color(in);
        if(out != out_predicted){
            printFail(test_name, "get_index_by_color(%06x) must be equals %u, but returns %u", in, out_predicted, out);
            return;
        }
    }

    printPass(test_name);
}

void test_number_to_color(){
    const char *test_name = "color_test.test_number_to_color";
    printInfo(test_name);

    //first argument - output, second - number (input), third - color_bit_resolution (input)
    uint32_t test_cases[][3]={
        {0xff0000, 0, 6},
        {0xf7ff00, 11, 6},
        {0x08ff00, 21, 6},
        {0x00ffff, 32, 6},
        {0x0800ff, 43, 6},
        {0xf700ff, 53, 6},
        {0xff0018, 63, 6},

        {0xff1800, 1, 6},
        {0xffef00, 10, 6},
        {0xdfff00, 12, 6},
        {0x20ff00, 20, 6},
        {0x00ff10, 22, 6},
        {0x00ffe7, 31, 6},
        {0x00e7ff, 33, 6},
        {0x0040ff, 40, 6},
        {0x0010ff, 42, 6},
        {0xdf00ff, 52, 6},
        {0xff0018, 63, 6},

        {0xff3000, 2, 6},
        {0xdf00ff, 52, 6},
        {0xff8f00, 6, 6},
        {0xf7ff00, 11, 6},
        {0x00ff40, 24, 6},
        {0xff0018, 63, 6},
        {0xff0078, 59, 6},
        {0xaf00ff, 50, 6},
        {0x00e7ff, 33, 6},
        {0xff0030, 62, 6}
    };

    uint32_t exception_cases[][2]={
        {64, 6},
        {65,6},
        {1200, 6},
        {1024, 11}
    };

    int test_count = sizeof(test_cases)/3/sizeof(uint32_t);

    for(int i = 0; i < test_count; i++){
        uint16_t number = (uint16_t)test_cases[i][1];
        int color_bit_resolution = (int)test_cases[i][2];
        uint32_t out_predicted = test_cases[i][0];

        try{
            uint32_t out = number_to_color(number, color_bit_resolution);
            if(out != out_predicted){
                printFail(test_name, "number_to_color(%u, %d) must return %x, but returns %x", number, color_bit_resolution, out_predicted, out);
                return;
            }
        }
        catch(status_error error){
            printFail(test_name, error.what());
            return;
        }
    }

    int exception_count = sizeof(exception_cases)/2/sizeof(uint32_t);
    for(int i = 0; i < exception_count; i++){
        uint16_t number = exception_cases[i][0];
        int color_bit_resolution = exception_cases[i][1];

        try{
            number_to_color(number, color_bit_resolution);
            printFail(test_name, "number_to_color(%u, %d) must throw an exception!", number, color_bit_resolution);
            return;
        }
        catch(status_error err) { }
        
    }

    printPass(test_name);
}

void test_color_to_number(){
    const char *test_name = "color_test.test_number_to_color";
    printInfo(test_name);
    uint32_t test_cases[][3] = {
        {63, 0x2b0709, 6},
        {38, 0x5089c6, 6},
        {63, 0xac131d, 6},
        {46, 0x5f28cc, 6},
        {62, 0xe95d72, 6},
        {62, 0x6e051c, 6},
        {22, 0x6eb273, 6},
        {34, 0x07abca, 6},
        {34, 0x4e757f, 6},
        {44, 0x060413, 6}
    };
    int test_count = sizeof(test_cases)/3/sizeof(uint32_t);

    for(int i = 0; i < test_count; i++){
        uint32_t color = (uint32_t)test_cases[i][1];
        int color_bit_resolution = (int)test_cases[i][2];
        uint16_t out_predicted = (uint16_t)test_cases[i][0];

        try{
            uint16_t out = color_to_number(color, color_bit_resolution);
            if(out != out_predicted){
                printFail(test_name, "color_to_number(0x%06x, %d) must return %u, but returns %u", color, color_bit_resolution, out_predicted, out);
                return;
            }
        }
        catch(status_error error){
            printFail(test_name, error.what());
            return;
        }
    }
    printPass(test_name);
}

void test_rgb_to_hex(){
    const char *test_name = "color_test.test_rgb_to_hex";
    printInfo(test_name);
    tuple<int, string> test_cases[] = {
        {0xff00aa, "#ff00aa"},
        {0xff00aa, "#ff00aa"},
        {0x000000, "#000000"},
        {0xffffff, "#ffffff"},
        {0x800000, "#800000"},
        {0x008000, "#008000"},
        {0x000080, "#000080"},
        {0x123456, "#123456"},
        {0xabcdef, "#abcdef"},
        {0x0f0f0f, "#0f0f0f"},
        {0xf0f0f0, "#f0f0f0"},
        {0x101010, "#101010"},
        {0x010203, "#010203"},
        {0xdeadbe, "#deadbe"},
        {0xc0ffee, "#c0ffee"},
        {0x777777, "#777777"},
        {0x00ff00, "#00ff00"},
        {0xff0000, "#ff0000"},
        {0x0000ff, "#0000ff"},
        {0x123abc, "#123abc"},
        {0xfedcba, "#fedcba"}
    };
    for(auto test_case : test_cases){
        int in = get<0>(test_case);
        string out_expected = get<1>(test_case);
        string out = rgb_to_hex(in);
        if(out != out_expected){
            fail(test_name, "Expected %s, but got %s!", 1, out_expected.c_str(), out.c_str());
        }
    }

    printPass(test_name);
}

int main(){
    test_get_color_by_index();
    test_get_index_by_color();
    test_number_to_color();
    test_color_to_number();
    test_rgb_to_hex();
    return 0;
}