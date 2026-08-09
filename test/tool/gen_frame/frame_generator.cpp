//input: --width|-W VALUE, --height|-H VALUE, --frame_codec_path|-c VALUE, 
//--expected_path|-e, --data_path|-d VALUE
//output: array of random expected blocks, path to frame in binary format
#include "config/data/video_config.hpp"
#include "config/parser/frame_codec/frame_codec_parser.hpp"
#include "frame_io.hpp"
#include "lib/CLI11.hpp"
#include "frame_meta.hpp"
#include "frame_expected_out_parser.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "io.hpp"
#include <cstring>
#include <random>

int main(int argc, char **argv) {
    CLI::App app{"frame_gen_tool"};
    argv = app.ensure_utf8(argv);
    frame_expected_in in;
    in.fps = 30;
    app.add_option("-W,--width", in.frame_width)->required();
    app.add_option("-H,--height", in.frame_height)->required();
    app.add_option("-f,--fps", in.fps);
    app.add_option("-c,--frame_codec_path", in.codec_path)->required();
    app.add_option("-e,--expected_path", in.expected_path)->required();
    app.add_option("-d,--data_path", in.data_path)->required();
    CLI11_PARSE(app, argc, argv);

    frame_expected_out out;
    memcpy((video_config*)&out, (video_config*)&in, sizeof(video_config));
    auto codec_parser = (frame_codec_parser*)frame_codec_parser_static_factory().build();
    auto out_parser  = new frame_expected_out_parser(codec_parser);
    frame_io io;
    string codec_path = DATA_FRAME_CODEC_PATH / to_native_path(in.codec_path);
    out.codec = io.read_codec_from_file(codec_path, in);

    std::random_device seed; 
    std::mt19937_64 gen(seed());
    std::uniform_int_distribution<> dis(-127, 128);
    for(int i = 0; i < out.codec -> payload_size(); i++){
        out.data.payload.push_back(dis(gen));
    }
    out.data.path = in.data_path;
    delete out_parser;

    string expected_path = EXPECTED_FRAME_PATH / to_native_path(in.expected_path);
    string data_path = DATA_FRAME_PATH / to_native_path(in.data_path);
    io.write_frame_expected(out, expected_path);
    char *frame = out.codec->encode(out.data.payload.data());
    io.write_frame_data(reinterpret_cast<uint8_t*>(frame), out.frame_width, out.frame_height, data_path);
    delete frame;
    return 0;
}