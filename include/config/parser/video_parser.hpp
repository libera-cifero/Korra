#include "../data/video_config.hpp"
#include "parser.hpp"

class root_parser : public json_parser<video_config>{
public:
    video_config parse(json) override;
    json serialize(video_config) override;
};