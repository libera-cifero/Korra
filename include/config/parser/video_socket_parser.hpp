#pragma once
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/pipe/in/video_pipe_in_parser.hpp"
#include "config/parser/pipe/out/video_pipe_out_parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "config/parser/tun_parser.hpp"
#include "config/parser/video_stream_in_parser.hpp"
#include "config/parser/video_stream_out_parser.hpp"
#include "parser.hpp"
#include "video_socket.hpp"

class video_socket_parser : public json_parser<video_socket*>
{
private:
    video_config_parser *_video_config_parser = nullptr;
    tun_parser *_tun_parser = nullptr;
    frame_codec_parser *_frame_codec_parser = nullptr;
    video_pipe_in_parser *_pipe_in_parser = nullptr;
    video_pipe_out_parser *_pipe_out_parser = nullptr;
    video_stream_in_parser *_stream_in_parser = nullptr;
    video_stream_out_parser *_stream_out_parser = nullptr;

    void _destroy_settings(video_socket_settings &settings);
    void _set_context_in(video_config context);
public:
    video_socket_parser(frame_codec_parser *, video_pipe_in_parser *, video_pipe_out_parser *);
    video_socket* parse(json) override;
    json serialize(video_socket*) override;
    ~video_socket_parser() override;
};