#pragma once
#include "video_encoder/frame_encoder/provider/len_reader/data_length_reader.hpp"
#include "config/parser/parser.hpp"

class raw_ip_length_reader_parser : public specific_parser<data_length_reader*> {
public:
    string type() override;
    data_length_reader* parse(json) override;
    json serialize(data_length_reader*) override;
};