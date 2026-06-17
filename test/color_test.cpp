#include "../include/color.h"
#include "test.h"
#include <cstdint>
#include <stdint.h>

void test_get_color_by_index(){
    const char *test_name = "test_get_color_by_index";
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

}

int main(){
    test_get_color_by_index();


    return 0;
}