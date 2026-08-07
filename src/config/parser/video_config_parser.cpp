#include "config/parser/video_config_parser.hpp"
#include "config/data/video_config.hpp"
#include "lib/json.hpp"
#include "lib/parsing.hpp"

video_config video_config_parser::parse(json object){
    return {
        .frame_width = parse_uint32_t(object["frameWidth"]),
        .frame_height = parse_uint32_t(object["frameHeight"]),
        .fps = parse_uint32_t(object["fps"])
    };
}

json video_config_parser::serialize(video_config config){
    json object = json::object({
        { "frameWidth", config.frame_width },
        { "frameHeight", config.frame_height },
        { "fps", config.fps }
    });
    return object;
}