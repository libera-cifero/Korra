#pragma once
#include "config/parser/encoder_parser.hpp"
#include "config/parser/pipes_parser.hpp"
#include "config/parser/video_parser.hpp"
#include "video_socket.hpp"
#include "parser.hpp"

class root_parser : public json_parser<video_listener*>{
private:
    video_parser *_video;
public:
    encoder_parser *encoder;
    pipes_parser *pipes;

    root_parser();
    video_listener* parse(json) override;
    json serialize(video_listener*) override;

    ~root_parser();
};