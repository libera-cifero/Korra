#include "config/parser/video_parser.hpp"
#include "config/config_error.hpp"
#include "status_error.hpp"
#include <cstdint>

video_config video_parser::parse(json root) {
    auto width_token = root["width"], height_token = root["height"], fps_token = root["fps"];

    if (!width_token.is_number_unsigned()) throw status_error("[config:video:width] width must be greater 0!", config_error::PARSE_VIDEO_WIDTH_INVALID);
    if (!height_token.is_number_unsigned()) throw status_error("[config:video:height] height must be greater 0!", config_error::PARSE_VIDEO_HEIGHT_INVALID);
    if (!fps_token.is_number_unsigned()) throw status_error("[config:video:fps] fps must be greater 0!", config_error::PARSE_VIDEO_FPS_INVALID);

    uint32_t width = width_token;
    uint32_t height = height_token;
    uint32_t fps = fps_token;

    return {
        .frame_width = width,
        .frame_height = height,
        .fps = fps
    };
}

json video_parser::serialize(video_config config) {
    if(config.frame_width == 0) throw status_error("[config:video:width] width must be greater 0!", config_error::SERIALIZE_VIDEO_WIDTH_INVALID);
    if(config.frame_height == 0) throw status_error("[config:video:width] width must be greater 0!", config_error::SERIALIZE_VIDEO_HEIGHT_INVALID);
    if(config.fps == 0) throw status_error("[config:video:width] width must be greater 0!", config_error::SERIALIZE_VIDEO_HEIGHT_INVALID);

    return {
        { "width",config.frame_width},
        { "height", config.frame_height }, 
        { "fps", config.fps}
    };
}