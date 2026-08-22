#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "config/parser/pipe/out/video_pipe_out_parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "parser.hpp"
#include "video_stream_out.hpp"

class video_stream_out_parser : public json_parser<video_stream_out*> {
private:
    frame_codec_parser *_frame_codec_parser;
    video_pipe_out_parser *_pipe_out_parser;
    video_config_parser *_video_config_parser;
public:
    video_stream_out_parser(video_config_parser *,frame_codec_parser *, video_pipe_out_parser *);
    video_stream_out *parse(json) override;
    json serialize(video_stream_out*) override;
};