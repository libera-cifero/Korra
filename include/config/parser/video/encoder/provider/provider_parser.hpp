#pragma once
#include "config/data/video_config.hpp"
#include "len_reader/len_reader_parser.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include "config/parser/parser.hpp"

class provider_parser : public abstract_parser<provider*>{
public:
    video_config *video;
    data_length_reader_parser *length_reader;

    provider_parser();

    provider* parse(json) override;
    json serialize(provider*) override;
};