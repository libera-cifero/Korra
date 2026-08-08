#include "video_codec/frame_codec/frame_codec.hpp"
#include "parser_factory.hpp"

class frame_codec_parser_static_factory : public parser_factory<frame_codec*> {
public:
    json_parser<frame_codec*>* build() override;
};