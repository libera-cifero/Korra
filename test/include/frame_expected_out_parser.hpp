#pragma once
#include "config/parser/parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "frame_meta.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"

class frame_expected_out_parser : public json_parser<frame_expected_out> {
private:
    frame_codec_parser *_codec_parser;
    video_config_parser *_config_parser;
public:
    frame_expected_out_parser(json_parser<frame_codec*> *);
    frame_codec_parser *codec_parser();
    video_config_parser *config_parser();
    frame_expected_out parse(json) override;
    json serialize(frame_expected_out) override;
    ~frame_expected_out_parser() override;
};