#pragma once
#include "../data/root_config.hpp"
#include "config/parser/encoder/provider/provider_parser.hpp"
#include "config/parser/encoder_parser.hpp"
#include "config/parser/pipes_parser.hpp"
#include "config/parser/video_parser.hpp"
#include "parser.hpp"

class root_parser : public json_parser<root_config>{
private:
    video_parser *_video;
public:
    encoder_parser *encoder;
    pipes_parser *pipes;

    root_parser();
    root_config parse(json) override;
    json serialize(root_config) override;

    ~root_parser();
};