#include "encoder_config.hpp"
#include "pipes_config.hpp"
#include "video_config.hpp"

struct root_config {
    video_config video;
    encoder_config encoder;
    pipes_config pipes;
};