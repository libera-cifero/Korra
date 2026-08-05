#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "lib/json.hpp"

using json=nlohmann::json;
block_codec *parse_block_codec(json &root, int frame_width, int frame_height, int block_size);
json serialize_block_codec(block_codec *codec);