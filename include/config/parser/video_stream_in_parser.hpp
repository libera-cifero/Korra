#pragma once
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/pipe/in/video_pipe_in_parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "parser.hpp"
#include "video_stream_in.hpp"

class video_stream_in_parser : public json_parser<video_stream_in*> {
private:
    frame_codec_parser *_frame_codec_parser;
    video_pipe_in_parser *_pipe_in_parser;
    video_config_parser *_video_config_parser;

    void _set_context_in();
public:
    video_stream_in_parser(video_config_parser *,frame_codec_parser *, video_pipe_in_parser *);
    video_stream_in *parse(json) override;
    json serialize(video_stream_in*) override;
};