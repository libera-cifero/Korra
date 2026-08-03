//TEST IT MANUALLY!!!
#include "frame_meta.hpp"
#include "test.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "video_codec/frame_codec/cipher/none_cipher.hpp"
#include "video_codec/frame_codec/frame_codec.hpp"
#include "video_codec/frame_codec/provider/mosaic/mosaic_settings.hpp"
#include "video_codec/payload_storage.hpp"
#include "video_codec/sync_signals.hpp"
#include "video_codec/video_codec.hpp"
#include "video_codec/frame_codec/provider/mosaic_provider.hpp"
#include "video_codec/frame_codec/provider/provider.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <regex>
#include <semaphore>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std;
using namespace filesystem;
const string base_name = "video_codec_test";

FILE *begin_video_making(int width, int height, int fps, string output){
    std::string cmd =
        "ffmpeg -y -f rawvideo -pix_fmt bgr24 "
        "-s " + std::to_string(width) + "x" + std::to_string(height) +
        " -r " + std::to_string(fps) +
        " -i - "
        "-vf vflip -c:v libx264 -g 1 -sc_threshold 0 -pix_fmt yuv420p -preset veryfast "
        "\"" + output + "\"";
    return popen(cmd.c_str(), "w");
}

void test_encode(){
    const string test_name = base_name + ".test_encode";
    printInfo(test_name.c_str());   

    bool inited = false;
    thread *t;
    payload_storage *storage;
    binary_semaphore sync(0), payloads_token(0);
    int counter = 0;

    vector<char*> frames;
    FILE *video_maker = begin_video_making(1280, 720, 30, DATA_OUT_PATH / "output.mp4");
    iterate_frame_test_cases(test_name.c_str(), "1280x720", [&](ITER_ACTION_ARGS) {
        if(!inited){
            t = new thread([&](frame_meta meta){
                mosaic_settings *settings = new mosaic_settings;
                *settings = meta;
                
                provider *p = new mosaic_provider(settings);
                cipher *c = new none_cipher(p->payload_size());
                frame_codec *encoder = new frame_codec(p, c);
                video_codec *video = new video_codec(encoder, 30, 14);
                storage = video->storage();
                sync.release();
                payloads_token.acquire();//waiting while payload will be full
                video->launch();
                do{
                    char *frame = storage->pop_frame();
                    frames.push_back(frame);
                    fwrite(frame, 1, storage->frame_size(), video_maker);
                }
                while(--counter > 0);

                delete video;
                sync.release();
            }, meta);
            t->detach();
            sync.acquire();
            inited = true;
        }

        char *buffer = storage->current_payload();
        char *payload = convert_blocks_to_data(meta.blocks, meta.codec->bits_per_number());
        memcpy(buffer, payload, storage->payload_size());
        storage->begin_new_payload();
        delete [] payload;
        if(counter++ > 0) delete meta.codec;
    });

    payloads_token.release();
    sync.acquire();//waiting until frames were be encoded
    delete t;

    pclose(video_maker);
    path p = DATA_OUT_PATH;
    if(!is_directory(p)) create_directory(p);
    p /= "frame";
    if(!is_directory(p)) create_directory(p);
    p /= "test_encode";
    if(!is_directory(p)) create_directory(p);

    for(int i = 0; i < frames.size(); i++){
        uint8_t *frame = reinterpret_cast<uint8_t*>(frames[i]);
        printInfo("index: %d", i);
        write_frame_data(frame, 1280, 720, p / format("frame_{}.bmp", i));
        delete [] frame;
    }
    frames.clear();

    char command[1024];
    p = DATA_OUT_PATH / "frames_outputmp4" / "frame_%04d.bmp";
    path p_in = DATA_OUT_PATH / "output.mp4";
    sprintf(command, "ffmpeg -i %s -vsync 0 %s", p_in.c_str(), p.c_str());
    system(command);;
    printPass(test_name.c_str());
}

void test_pop_frame(){
    const string test_name = base_name + ".test_pop_frame";
    printInfo(test_name.c_str());
    iterate_frame_test_cases(test_name.c_str(), "1280x720", [](ITER_ACTION_ARGS) {
        mosaic_settings *settings = new mosaic_settings;
        memcpy(settings, (mosaic_settings*)&meta, sizeof(mosaic_settings));
        provider *p = new mosaic_provider(settings);
        cipher *c = new none_cipher(p->payload_size());
        frame_codec *encoder = new frame_codec(p, c);
        sync_signals *signals = new sync_signals;
        payload_storage *storage = new payload_storage(encoder, signals);

        char *payload = convert_blocks_to_data(meta.blocks, meta.codec->bits_per_number());
        char *buffer = storage->current_payload();
        memcpy(buffer, payload, storage->payload_size());
        delete [] payload;
        binary_semaphore sync(0);
        thread t([&]() 
        {
            regex r("frame_\\d+");
            smatch m;
            regex_match(meta.frame_path, m, r);
            regex_search(meta.frame_path, m, r);
            path p = DATA_OUT_PATH;
            if(!is_directory(p)) create_directory(p);
            p /= "frame";
            if(!is_directory(p)) create_directory(p);
            p /= "test_pop_frame";
            if(!is_directory(p)) create_directory(p);
            p /= (m.str()+".bmp");
            sync.release();
            char *frame = storage->pop_frame();
            write_frame_data(reinterpret_cast<uint8_t*>(frame), meta.frame_width, meta.frame_height, p);
            delete[] frame;
            sync.release();
        });

        t.detach();
        sync.acquire();
        signals->ready_request()->release();
        signals->ready_response()->acquire();
        signals->frame_request()->release();
        sync.acquire();
        delete storage;
        delete encoder;
        delete signals;
    });
    printPass(test_name.c_str());
}

int main(){
    test_encode();
    test_pop_frame();
    return 0;
}