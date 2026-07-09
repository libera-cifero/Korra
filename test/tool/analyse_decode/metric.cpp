//input: file_in file_out
//output: error_percent average_error median_error decoding_time_ms
#include "encoder/provider/basic_block/basic_block_container.hpp"
#include "frame_io.hpp"
#include "frame_meta.hpp"
#include "io.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

vector<string> parse_hyphen_ranges(string range, string extension) {
    vector<string> files;
    char buff[256];
    sprintf(buff, "frame_(\\d+)\\.%s\\-frame_(\\d+)\\.%s", extension.c_str(), extension.c_str());
    regex pattern(buff);
    
    for (auto it = sregex_iterator(range.begin(), range.end(), pattern); it != sregex_iterator(); ++it) {
        smatch match = *it;
        int begin = atoi(match[1].str().c_str());
        int end = atoi(match[2].str().c_str());
        for(int i = begin; i <= end; i++){
            string file = "frame_" + to_string(i) + "." + extension;
            files.push_back(file);
        }
    }

    return files;
}

vector<string> parse_comma_ranges(string range, string extension) {
    vector<string> files;
    char buff[256];
    sprintf(buff, "(?<=,|^)(\\w+\\.%s)(?!\\-)", extension.c_str());
    regex pattern(buff, regex_constants::multiline);

    for (auto it = sregex_iterator(range.begin(), range.end(), pattern); it != sregex_iterator(); ++it) {
        smatch match = *it;
        string file = match[1].str();
        files.push_back(file);
    }

    return files;
}

vector<string> parse_file_range(string range, string file_extension) {
    vector<string> comma_files = parse_comma_ranges(range, file_extension);
    vector<string> hyphen_files = parse_hyphen_ranges(range, file_extension);
    vector<string> files;
    for(string file : comma_files) files.push_back(file);
    for(string file : hyphen_files) files.push_back(file);
    return files;
}

int main(int argc, char **argv) {
    if(argc < 3){
        throw runtime_error("Too few arguments!");
    }

    string path = EXPECTED_FRAME_PATH / argv[1];
    frame_meta args = read_frame_expected(path);
    string frame_path = DATA_FRAME_PATH / args.frame_path;
    int width, height;
    uint8_t *data_frame = read_frame_data(frame_path, width, height);
    basic_block_container frame0(data_frame, args);

    string frame_path1 = DATA_FRAME_PATH / argv[2];
    uint8_t *data_frame1 = read_frame_data(frame_path1, width, height);
    basic_block_container frame1(data_frame1, args);


    if(frame0.byte_count() != frame1.byte_count()){
        delete [] data_frame1;
        delete [] data_frame;
        throw runtime_error("first byte_count and second byte_count aren't equal!");
    }

    int byte_count = frame0.byte_count();
    uint8_t *data0 = new uint8_t[byte_count], *data1 = new uint8_t[byte_count];
    timespec t0, t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    frame0.read(frame0.begin(), data0, byte_count);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    frame1.read(frame1.begin(), data1, byte_count);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    double elapsed0_ms = ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
    double elapsed1_ms = ((t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9) * 1000.0;
    double elapsed_ms = (elapsed0_ms + elapsed1_ms) / 2;

    int error_count = 0, average_delta_sum = 0, median_error;

    for(int i = 0; i < byte_count; i++){
        uint8_t byte0 = data0[i], byte1 = data1[i];
        if(byte0 != byte1){
            error_count++;
            cout << i << " " << (int)byte0 << ", " << (int)byte1 << endl;
            average_delta_sum += abs(byte1 - byte0);
        }
    }

    float error_percent = error_count / (float)byte_count * 100;
    float average_delta = average_delta_sum / (float)error_count;

    cout << error_percent << " " << average_delta << " " << elapsed_ms;

    error_count = average_delta_sum = 0;
    int block_count = frame0.block_count();
    for(int i = 0; i < block_count; i++){
        int block0 = frame0[i], block1 = frame1[i];
        if(block0 != block1){
            error_count ++;
            average_delta_sum += abs(block1 - block0);
        }
    }

    error_percent = error_count / (float)block_count * 100;
    average_delta = average_delta_sum / (float)error_count;

    cout << endl << error_percent << " " << average_delta<<endl;

    return 0;
}