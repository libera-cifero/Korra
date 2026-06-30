#include "test.h"
#include "math.h"
#include <cstddef>
#include <tuple>

using namespace std;

void test_get_lcm(){
    const char *test_name = "math_test.test_get_lcm";
    printInfo(test_name);
    tuple<size_t, size_t, size_t, size_t> test_cases[]={
        {208, 8, 13, 200},
        {432, 6, 16, 400},
        {1001, 7, 13, 1001},
        
        {24, 8, 12, 20},
        {24, 8, 12, 24},
        {48, 8, 12, 25},
        {72, 8, 12, 50},

        {42, 6, 7, 40},
        {84, 6, 7, 43},
        {126, 6, 7, 100},

        {60, 12, 15, 1},
        {60, 12, 15, 60},
        {120, 12, 15, 61},

        {36, 9, 12, 30},
        {72, 9, 12, 40},

        {30, 5, 6, 29},
        {60, 5, 6, 31},
        {90, 5, 6, 70},

        {77, 7, 11, 70},
        {154, 7, 11, 100},

        {91, 13, 7, 90},
        {182, 13, 7, 92},

        {221, 13, 17, 200},
        {442, 13, 17, 222},

        {210, 14, 15, 200},
        {420, 14, 15, 211},

        {64, 8, 16, 50},
        {64, 8, 16, 64},
        {80, 8, 16, 65},

        {18, 6, 9, 1},
        {18, 6, 9, 18},
        {36, 6, 9, 19},

        {100, 20, 25, 99},
        {200, 20, 25, 101},

        {1001, 7, 13, 1001},
        {1092, 7, 13, 1002},

        {208, 8, 13, 200},
        {312, 8, 13, 209},

        {432, 6, 16, 400},
        {480, 6, 16, 433},

        {504, 18, 28, 500},
        {756, 18, 28, 505},

        {315, 9, 35, 300},
        {630, 9, 35, 316},

        {143, 11, 13, 140},
        {286, 11, 13, 144},

        {323, 17, 19, 320},
        {646, 17, 19, 324},

        {840, 24, 35, 800},
        {1680, 24, 35, 841}
    };
    
    for(auto test_case : test_cases){
        size_t out_expected = get<0>(test_case), a = get<1>(test_case), b = get<2>(test_case), min_res = get<3>(test_case);

        size_t out = get_lcm(a, b, min_res);
        if(out != out_expected){
            fail(test_name, "Expected %u, but got %u", 1, out_expected, out);
        }
    }

    printPass(test_name);
}

int main(){
    test_get_lcm();
    return 0;
}