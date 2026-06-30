#include "encoder/provider/basic_block_config.h"
#include <vector>
struct frame_meta : basic_block_config {
    char *frame_path;
    std::vector<int> blocks;
};