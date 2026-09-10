#include "config/data/video_config.hpp"
#include "pipe/in/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in_unix.hpp"
#include "pipe/out/pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out_unix.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include "io.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <ios>
#include <string>
#include <thread>
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

void test_read(){
    const char *test_name = "ffmpeg_rtmp_pipe_in.test_read";
    frame_io context;
    ffmpeg_rtmp_config config;
    frame_codec *codec = nullptr;
    bool config_inited = false;
    vector<char*> frames;
    vector<char*> payloads;
    int index = 0, frame_size, payload_size;
    context.iterate_frame_test_cases(test_name, "rgb_palette0/1280x720", [&](ITER_ACTION_ARGS){
        printInfo("reading frame %d to buffer", index++);
        bool is_first = !config_inited;
        if(!config_inited){
            config.frame_height = meta.frame_height;
            config.frame_width = meta.frame_width;
            config.fps = meta.fps;
            codec = meta.codec;
            frame_size = codec->frame_size();
            payload_size = codec->payload_size();
            config_inited = true;
        }

        char *data1 = new char[frame_size];
        memcpy(data1, data, frame_size);

        frames.push_back(data1);
        if(!is_first) delete meta.codec;
    });

    config.rtmp_url = read_link(DATA_SECRET_PATH / "ffmpeg_rtmp_pipe_out_test.test_write.link");
    ffmpeg_rtmp_pipe_in *pipe_in = new ffmpeg_rtmp_pipe_in_unix(config);
    ffmpeg_rtmp_pipe_out *pipe_out = new ffmpeg_rtmp_pipe_out_unix(config);
    bool success = true;
    int fail_index = -1;
    thread out([&](){
        this_thread::sleep_for(chrono::milliseconds(5000));
        for(int i = 0; i < frames.size(); i++){
            pipe_out->write(frames[i], frame_size);
        }
        printWarning("Writen!");
        //delete pipe_out;
    });
    int frame_count = frames.size();
    for(int i = 0; i < frame_count && success; i++){
        auto frame = frames[i];
        char *frame_received = pipe_in->read(frame_size);
        printInfo("frame %d/%d", i, frame_count);
        //log_received_frame(frame_received, config, i);
        char *data_sent = codec->decode(reinterpret_cast<char*>(frame));
        char *data_received = codec->decode(reinterpret_cast<char*>(frame_received));
        success = memcmp(data_sent, data_received, payload_size) == 0;
        log_diffs(data_sent, data_received, payload_size, i);
        if(!success) fail_index = i;
        delete [] data_sent;
        delete [] data_received;
        delete [] frame;
        delete [] frame_received;
    }
    out.join();
    delete codec;
    delete pipe_out;
    delete pipe_in;

    if(success) printPass(test_name);
    else fail(test_name, "frame sent and frame received by index %d don't equal!", 1, fail_index);
}

int main(){
    test_read();
    return 0;
}