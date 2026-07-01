#include "encoder/provider/basic_block_config.h"
#include <vector>
#include <string>

struct frame_meta : basic_block_config {
    std::string frame_path;
    std::vector<int> blocks;
};

struct frame_gen_args : basic_block_config { 
    std::string expected_name;
    std::string frame_name;
};