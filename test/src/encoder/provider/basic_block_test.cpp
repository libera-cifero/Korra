#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "frame_meta.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "encoder/provider/basic_block/basic_block_pointer.hpp"
#include "encoder/provider/basic_block_config.hpp"
#include "test.h"
#include <cstdint>
#include <exception>
#include <filesystem>
#include <regex>
#include <stdint.h>
#include <string>

#define TEST_BASE_NAME "basic_block_test"
#define BBC_TEST_BASE_NAME TEST_BASE_NAME ".basic_block_container"
#define PTR_TEST_BASE_NAME TEST_BASE_NAME ".basic_block_pointer"
using namespace std;
using namespace filesystem;

//basic_block_pointer
//=====BEGIN=====
void test_ptr_equal(){
    const char *test_name = PTR_TEST_BASE_NAME ".test_ptr_equal";
    printInfo(test_name);
    basic_block_config config = {
        .block_size = 4,
        .frame_width = 10,
        .frame_height = 12,
        .codec = NULL
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

void iterate_bbc_test_cases(const char *test_name, void (*test)(const char *test_name, frame_meta expected, uint8_t *data, string file_name)){
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

            test(test_name, expected, data, file_name);

            delete[] data;
        }
    }
}

void test_get_block_by_index() {
    const char *test_name = BBC_TEST_BASE_NAME ".test_get_block_by_index";
    printInfo(test_name);

    iterate_bbc_test_cases(test_name, [](const char *test_name, frame_meta expected, uint8_t *data, string file_name)
    {
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
    });

    printPass(test_name);
}

void test_set_block_by_index(){

}

void test_block_byte_io() {
    const char *test_name = BBC_TEST_BASE_NAME ".test_block_byte_io";
    printInfo(test_name);
    srand(time(NULL));
    iterate_bbc_test_cases(test_name, [](const char *test_name, frame_meta expected, uint8_t *data, string file_name) 
    {
        const char *cfile_name = file_name.c_str();
        basic_block_container container(data, expected);
        size_t byte_count = container.byte_count();
        uint8_t *read_buffer0 = (uint8_t*)calloc(byte_count, 1);
        auto end = container.read(container.begin(), read_buffer0, byte_count);
        if(end != nullptr) {
            delete[] read_buffer0;
            delete[] data;
            printInfo("[read0] %s", cfile_name);
            fail(test_name, "end pointer must equals nullptr!", 10);
        }

        for(int i = 0; i < byte_count; i++) read_buffer0[0] = rand() % 256;
        
        end = container.write(container.begin(), read_buffer0, byte_count);

        if(end != nullptr) {
            delete[] read_buffer0;
            delete[] data;
            printInfo("[write0] %s", cfile_name);
            fail(test_name, "end pointer must equals nullptr!", 11);
        }

        uint8_t *read_buffer1 = (uint8_t*)calloc(byte_count, 1);
        container.read(container.begin(), read_buffer1, byte_count);
        for(int i = 0; i < byte_count; i++){
            if(read_buffer0[i] != read_buffer1[i]){
                delete [] read_buffer0;
                delete [] read_buffer1;
                delete [] data;
                printInfo(cfile_name);
                fail(test_name, "read_buffer0[%d] != read_buffer1[%d]", 12, i);
            }
        }

        delete [] read_buffer0;
        delete [] read_buffer1;
    });

    printPass(test_name);
}
//======END======

int main() {
    try{
        test_ptr_equal();

        test_get_block_by_index();
        test_set_block_by_index();
        
        test_block_byte_io();
    }
    catch(exception &e){
        printFail(TEST_BASE_NAME, e.what());
        return -1;
    }
    return 0;
}