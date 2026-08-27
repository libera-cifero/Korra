#include "config/data/video_config.hpp"
#include "pipe/out/pipe/ffmpeg_rtmp_pipe_out.hpp"
#include "test.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <thread>
#include <tuple>
#include <filesystem>
using namespace filesystem;
string read_link(path link_path){
    ifstream file(link_path);
    string link;
    getline(file, link);
    file.close();
    return link;
}

void test_write(){
    const char *test_name = "ffmpeg_rtmp_pipe_out.test_write";
    printInfo(test_name);
    frame_io context;
    vector<tuple<uint8_t *, int>> frames;
    ffmpeg_rtmp_config config;
    bool config_inited = false;
    printInfo("reading frames...");
    int index = 0;
    context.iterate_frame_test_cases(test_name, "rgb_xchacha20/1280x720", [&](ITER_ACTION_ARGS){
        int size = meta.codec->frame_size();
        uint8_t *data1 = new uint8_t[size];
        memcpy(data1, data, size);
        frames.push_back({data1, size});
        if(!config_inited){
            memcpy((video_config*)&config, (video_config*)&meta, sizeof(video_config));
            config_inited = true;
        }
        printInfo("frame %d", index++);
        delete meta.codec;
    });
    config.rtmp_url = read_link(DATA_SECRET_PATH / "ffmpeg_rtmp_pipe_out_test.test_write.link");
    ffmpeg_rtmp_pipe_out *out = new ffmpeg_rtmp_pipe_out(config);
    //auto delay = chrono::milliseconds(1000 / config.fps);
    printInfo("writing the frames...");
    for(int i = 0; i<frames.size(); i++){
        uint8_t *frame = get<0>(frames[i]);
        int size = get<1>(frames[i]);
        printInfo("frame %d", i);
        out->write(frame, size);
        //this_thread::sleep_for(delay);
        delete [] frame;
    }
    delete out;
    printPass(test_name);
}

int main(){
    test_write();
    return 0;
}