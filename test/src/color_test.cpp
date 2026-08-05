#include "lib/color.hpp"
#include "test.hpp"
#include <stdint.h>

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
    test_rgb_to_hex();
    return 0;
}