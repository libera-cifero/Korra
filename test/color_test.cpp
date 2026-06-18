#include "../include/color.h"
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
        //control points
        {0x520808, 0, 6},
        {0x141410, 11, 6},
        {0xa6e3a4, 21, 6},
        {0x037e7e, 32, 6},
        {0x141091, 43, 6},
        {0x433e43, 53, 6},
        {0x84131d, 63, 6},

        //around control points
        {0xcaa29e, 1, 6},
        {0x4e4e45, 10, 6},
        {0x303509, 12, 6},
        {0x3f7538, 20, 6},
        {0x445545, 22, 6},
        {0x000202, 31, 6},
        {0x7b9598, 33, 6},
        {0x2d313e, 40, 6},
        {0x4d5078, 42, 6},
        {0x321835, 52, 6},
        {0xdac8ca, 63, 6},

        //random points
        {0xa72e2e, 0, 6},
        {0x016eb0, 36, 6},
        {0xd99e7a, 4, 6},
        {0x124711, 21, 6},
        {0x99e5c1, 27, 6},
        {0xf3f9ed, 16, 6},
        {0x0c1309, 19, 6},
        {0xcaad74, 7, 6},
        {0x100f10, 49, 6},
        {0x7b77f6, 43, 6},
    };

    uint32_t exception_cases[][2]={
        {64, 6},
        {65,6},
        {1200, 6},
        {1024, 11}
    };

    int test_count = sizeof(test_cases)/3/sizeof(uint32_t);

    for(int i = 0; i < test_count; i++){
        test_cases[i][0] = get_color_by_index(number_to_index(test_cases[i][1], test_cases[i][2]));
    }

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

}

int main(){
    test_get_color_by_index();
    test_get_index_by_color();
    test_number_to_color();
    test_color_to_number();
    return 0;
}