#include "config/data/video_config.hpp"
#include "config/parser/pipe/in/ffmpeg_rtmp_pipe_in_parser.hpp"
#include "config/parser/pipe/out/ffmpeg_rtmp_pipe_out_parser.hpp"
#include "config/parser/video_config_parser.hpp"
#include "event/event_loop.hpp"
#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp"
#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in_unix.hpp"
#include "pipe/out/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp"
#include "pipe/out/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out_unix.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "lib/color.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include "io.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <ios>
#include <spdlog/fmt/bundled/format.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
using namespace filesystem;

struct test_case {
    uint8_t *frame;
    frame_codec *codec;
};

string read_link(path link_path){
    ifstream file(link_path);
    string link;
    getline(file, link);
    file.close();
    return link;
}

vector<int> find_diff_indices(char *payload0, char *payload1, int payload_size){
    vector<int> diffs;
    for(int i = 0; i<payload_size; i++){
        if(payload0[i] != payload1[i]) diffs.push_back(i);
    }
    return diffs;
}

int find_index(char *sequence, int sequence_size, vector<char*> &payloads) {
    for(int i = 0; i < payloads.size(); i++){
        if(memcmp(sequence, payloads[i], sequence_size) == 0) return i;
    }

    return -1;
}

void log_received_frame(uint8_t *frame_received, video_config &config, int index){
    string file_name = format("frame_{}.bmp", index);
    path dir = DATA_OUT_PATH;
    if(!is_directory(dir)) create_directory(dir);
    dir /= "ffmpeg_rtmp_pipe_in_test";
    if(!is_directory(dir)) create_directory(dir);
    dir /= file_name;
    frame_io context;
    context.write_frame_data(frame_received,config.frame_width , config.frame_height, dir);
}

void log_diffs(char *payload0, char *payload1, int payload_size, int index){
    string file_name = "diffs.txt";
    path dir = DATA_OUT_PATH;
    if(!is_directory(dir)) create_directory(dir);
    dir /= "ffmpeg_rtmp_pipe_in_test";
    if(!is_directory(dir)) create_directory(dir);
    dir /= file_name;
    frame_io context;

    vector<int> diffs = find_diff_indices(payload0, payload1, payload_size);
    ios_base::openmode mode = index == 0 ? ios_base::out : ios_base::app;
    ofstream out(dir, mode);
    out << "INDEX " << index << endl;
    for(int i = 0; i < diffs.size(); i++){
        out << diffs[i] << " ";
    }
    out.close();
}

void parse_pipes(string file_name,video_config *cfg, ffmpeg_rtmp_pipe_in **pipe_in, ffmpeg_rtmp_pipe_out **pipe_out){
    path file_path = CONTEXT_PATH / "config" / "pipe" / file_name;
    ifstream file(file_path);
    stringstream buf;
    buf << file.rdbuf();
    string config_text = buf.str();
    file.close();

    json config = json::parse(config_text);
    video_config_parser cfg_parser;
    ffmpeg_rtmp_pipe_in_parser pipe_in_parser;
    ffmpeg_rtmp_pipe_out_parser pipe_out_parser;
    *cfg = cfg_parser.parse(config);
    pipe_in_parser.context_in = *cfg;
    pipe_out_parser.context_in = *cfg;
    *pipe_in = (ffmpeg_rtmp_pipe_in*)pipe_in_parser.parse(config["pipe"]["in"]);
    *pipe_out = (ffmpeg_rtmp_pipe_out*)pipe_out_parser.parse(config["pipe"]["out"]);
}

uint32_t get_frame_color(char *frame, int frame_size){
    uint32_t red_sum = 0, green_sum = 0, blue_sum = 0;
    uint8_t *uframe = reinterpret_cast<uint8_t*>(frame);
    for(int i = 0; i < frame_size; i+=3){
        blue_sum += uframe[i];
        green_sum += uframe[i+1];
        red_sum += uframe[i + 2];
    }
    
    int px_count = frame_size / 3;
    red_sum /= px_count;
    green_sum /= px_count;
    blue_sum /= px_count;
    return (red_sum << 16) | green_sum << 8 | blue_sum;
}

char *make_color_frame(uint32_t color, int frame_size){
    auto r = get_r(color), g = get_g(color), b = get_b(color);
    char *frame = new char[frame_size];
    for(int i = 0; i < frame_size; i+=3) 
    {
        frame[i] = b;
        frame[i+1] = g;
        frame[i+2] = r;
    }
    return frame;
}

bool select_color(uint32_t received_color, vector<uint32_t> &colors, uint32_t &selected_color){

}

void test_read(){
    const char *test_name = "ffmpeg_rtmp_pipe_in.test_read";
    ffmpeg_rtmp_pipe_in *pipe_in;
    ffmpeg_rtmp_pipe_out *pipe_out;
    video_config config;
    parse_pipes("ffmpeg_rtmp0.json", &config, &pipe_in, &pipe_out);
    vector<uint32_t> colors = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFF6000, 0XFF0060 };
    int frame_size = config.frame_width * config.frame_height * 3;
    for(auto color : colors){
        char *frame = make_color_frame(color, frame_size);
        pipe_out->write(frame, frame_size);
        delete [] frame;
        frame = nullptr;
        do{
            frame = pipe_in->read(frame_size);
        }
        while(frame == nullptr);

    }
    printPass(test_name);
}

int main(){
    test_read();
    return 0;
}