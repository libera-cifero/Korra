#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "frame_meta.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.h"
#include "test.h"
#include <cstdint>
#include <exception>
#include <filesystem>
#include <regex>
#include <string>

#define TEST_BASE_NAME "basic_block_test"
#define PTR_TEST_BASE_NAME TEST_BASE_NAME ".basic_block_pointer"
using namespace std;
using namespace filesystem;

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

    directory_iterator iter(EXPECTED_PATH);
    regex pattern("frame_[0-9]+\\.json");
    for(directory_entry entry : iter) {
        smatch match;
        path file_path = entry.path();
        string file_name = file_path.filename();
        if(entry.is_regular_file() && regex_match(file_name, match, pattern)){
            int width, height;
            frame_meta expected = read_frame_expected(file_path);
            string frame_data_path = DATA_PATH / expected.frame_path;
            uint8_t *data = read_frame_data(frame_data_path, width, height);
            if(expected.frame_width != width){
                delete[] data;
                printInfo(file_name.c_str());
                fail(test_name, "Width is expected %d but got %d!", 1, expected.frame_width, width);
            }
            if(expected.frame_height != height){
                delete[] data;
                printInfo(file_name.c_str());
                fail(test_name, "Height is expected %d but got %d!", 2, expected.frame_height, height);
            }

            basic_block_container container(data, expected);
            if(container.block_count() != expected.blocks.size()){
                delete[] data;
                printInfo(file_name.c_str());
                fail(test_name, "Count of blocks is expected %d, but got %d!", 3, expected.blocks.size(), container.block_count());
            }
            for(int i = 0; i < container.block_count(); i++){
                int block = container[i];
                int block_expected = expected.blocks[i];
                if(block != block_expected){
                    delete[] data;
                    printInfo(file_name.c_str());
                    fail(test_name, "container[%d] is expected %d but got %d!", 4, i, block_expected, block);
                }
            }
            delete[] data;
        }
    }

    printPass(test_name);
}

void test_set_block_by_index(){

}
//======END======

int main() {
    try{
        test_get_block_by_index();
        test_set_block_by_index();

        test_ptr_equal();
    }
    catch(exception &e){
        printFail(TEST_BASE_NAME, e.what());
        return -1;
    }
    return 0;
}