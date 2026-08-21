#include "config/parser/pipe/out/ffmpeg_rtmp_pipe_out_parser.hpp"
#include "config/data/video_config.hpp"
#include "pipe/out/pipe/ffmpeg_rtmp_pipe_out.hpp"
#include "pipe/out/pipe/video_pipe_out.hpp"
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <string>

using namespace filesystem;
string ffmpeg_rtmp_pipe_out_parser::type(){
    return "ffmpeg_rtmp";
}

bool ffmpeg_rtmp_pipe_out_parser::can_serialize(video_pipe_out *pipe){
    return dynamic_cast<ffmpeg_rtmp_pipe_out*>(pipe) != nullptr;
}

video_pipe_out *ffmpeg_rtmp_pipe_out_parser::parse(json j){
    ffmpeg_rtmp_config config;
    memcpy((video_config*)&config, &context_in, sizeof(video_config));

    if(j.contains("rtmpUrl") && j["rtmpUrl"].is_string()) {
        config.rtmp_url = j["rtmpUrl"];
    }
    else if(j.contains("rtmpUrlFile") && j["rtmpUrlFile"].is_string()){
        path p = path(j["rtmpUrlFile"].get<string>());
        if(p.is_relative()) p = current_path() / p;
        ifstream file(p);
        getline(file, config.rtmp_url);
        file.close();
    }
    else throw runtime_error("ffmpeg_rtmp_pipe_out_parser.parse: It can't parse json! rtmpUrl, rtmpUrlFile are undefined or not strings!");

    return new ffmpeg_rtmp_pipe_out(config);
}

json ffmpeg_rtmp_pipe_out_parser::serialize(video_pipe_out *p) {
    ffmpeg_rtmp_pipe_out *pipe = static_cast<ffmpeg_rtmp_pipe_out*>(p);
    ffmpeg_rtmp_config config;
    memcpy((video_config*)&config, &context_in, sizeof(video_config));
    config.rtmp_url = pipe->url();

    return json::object({{"rtmpUrl", config.rtmp_url}});
}