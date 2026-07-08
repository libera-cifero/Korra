//input: file_in file_out
//output: error_percent average_error median_error decoding_time_ms
#include "encoder/provider/basic_block/basic_block_container.hpp"
#include <cstdio>
#include <cstdlib>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct metric_input {
    string in_expected;
    string in_frame;
    string out_frame;
};

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

metric_input parse_argv(int argc, char **argv){
    if(argc < 3){
        throw runtime_error("Too few arguments!");
    }

    return {
        .in_expected = string(argv[1]),
        .out_frame = string(argv[2])
    };
}

int main(int argc, char **argv) {
    metric_input args = parse_argv(argc, argv);
    

    return 0;
}