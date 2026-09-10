#include "config/parser/pipe/in/ffmpeg_rtmp_pipe_in_parser.hpp"
#include "config/data/video_config.hpp"
#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in_unix.hpp"
#include "pipe/in/pipe/video_pipe_in.hpp"
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <stdexcept>

using namespace filesystem;
string ffmpeg_rtmp_pipe_in_parser::type(){
    return "ffmpegRtmp";
}

bool ffmpeg_rtmp_pipe_in_parser::can_serialize(video_pipe_in *pipe){
    return dynamic_cast<ffmpeg_rtmp_pipe_in*>(pipe) != nullptr;
}

video_pipe_in *ffmpeg_rtmp_pipe_in_parser::parse(json j){
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
    else throw runtime_error("ffmpeg_rtmp_pipe_in_parser.parse: It can't parse json! rtmpUrl, rtmpUrlFile are undefined or not strings!");

#ifdef __linux__
    return new ffmpeg_rtmp_pipe_in_unix(config);
#elif _WIN32
    throw std::runtime_error("Unsupported OS");
#else
    throw runtime_error("Unsupported OS!");
#endif
}

json ffmpeg_rtmp_pipe_in_parser::serialize(video_pipe_in *p) {
    ffmpeg_rtmp_pipe_in *pipe = static_cast<ffmpeg_rtmp_pipe_in*>(p);
    ffmpeg_rtmp_config config;
    memcpy((video_config*)&config, &context_in, sizeof(video_config));
    config.rtmp_url = pipe->url();

    return json::object({{"rtmpUrl", config.rtmp_url}});
}