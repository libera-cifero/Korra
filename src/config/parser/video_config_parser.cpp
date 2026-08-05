#include "config/parser/video_config_parser.hpp"
#include "config/data/video_config.hpp"

video_config video_config_parser::parse(json object){
    return {
        .frame_width = object["frameWidth"],
        .frame_height = object["frameHeight"],
        .fps = object["fps"]
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