#include "config/parser/video_codec/frame_codec/provider/provider_parser.hpp"
#include "config/parser/video_codec/frame_codec/provider/len_reader/len_reader_parser.hpp"

provider_parser::provider_parser(){
    length_reader = new data_length_reader_parser;
}