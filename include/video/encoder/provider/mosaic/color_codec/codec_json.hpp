#include "color_codec.hpp"
#include "json.hpp"

using json=nlohmann::json;
color_codec *parse_color_codec(json &root);
json serialize_color_codec(color_codec *codec);