#include "encoder/provider/basic_block_config.h"
#include <vector>
#include <string>

struct frame_meta : basic_block_config {
    std::string frame_path;
    std::vector<int> blocks;
};