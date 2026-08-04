#include "video_codec/frame_codec/provider/mosaic/block_codec/block_codec.hpp"
#include "lib/json.hpp"

using json=nlohmann::json;
block_codec *parse_block_codec(json &root);
json serialize_block_codec(block_codec *codec);