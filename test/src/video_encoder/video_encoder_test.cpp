#include "frame_meta.hpp"
#include "test.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "video_encoder/frame_encoder/cipher/none_cipher.hpp"
#include "video_encoder/frame_encoder/frame_encoder.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/mosaic_settings.hpp"
#include "video_encoder/payload_storage.hpp"
#include "video_encoder/sync_signals.hpp"
#include "video_encoder/video_encoder.hpp"
#include "video_encoder/frame_encoder/provider/mosaic_provider.hpp"
#include "video_encoder/frame_encoder/provider/provider.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <regex>
#include <semaphore>
#include <string>
#include <thread>
#include <unistd.h>
using namespace std;
using namespace filesystem;
const string base_name = "video_encoder_test";

void test_encode(){
    const string test_name = base_name + ".test_pop_frame";
    printInfo(test_name.c_str());   

    bool encode = true, inited = false;
    binary_semaphore init(0), begin_encoding(0);
    payload_storage *storage = nullptr;
    thread t;
    int counter = 0;
    iterate_frame_test_cases(test_name.c_str(), "1280x720", [&](ITER_ACTION_ARGS) {
        if(!inited){
            t = thread([&](mosaic_settings settings){
                char command[256];
                sprintf(command, "ffmpeg -f rawvideo -pix_fmt bgr24 -s %dx%d -framerate %d -i - -c:v libx264 -pix_fmt yuv420p test/context/data/output.mp4", 
                    meta.frame_width, meta.frame_height, meta.fps);
                FILE* pipe = popen(command, "w");

                provider *p = new mosaic_provider(&settings);
                cipher *c = new none_cipher(p->payload_size());
                frame_encoder *f = new frame_encoder(p, c);
                video_encoder *encoder = new video_encoder(f, meta.fps, 15);
                storage = encoder->storage();
                inited = true;
                init.release();
                begin_encoding.acquire();
                encoder->launch();
                int wrote = 0;
                while(counter-- > 0) {
                    char *frame = storage->pop_frame();
                    fwrite(frame, storage->frame_size(), 1, pipe);
                    delete [] frame;
                    wrote++;
                    printInfo("Wrote frame %d!", wrote);
                }
                pclose(pipe);
                delete encoder;
                init.release();
            }, meta);
            t.detach();
            init.acquire();
        }
        
        char *payload = convert_blocks_to_data(meta.blocks, meta.codec->bits_per_number());
        storage->payload_index = 0;
        char *storage_payload = storage->current_payload();
        memcpy(storage_payload, payload, storage->payload_size());
        storage->begin_new_payload();
        storage->begin_frame_updating();
        delete payload;
        counter++;
    });

    begin_encoding.release();
    init.acquire();
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
        frame_encoder *encoder = new frame_encoder(p, c);
        sync_signals *signals = new sync_signals;
        payload_storage *storage = new payload_storage(encoder, signals);

        char *payload = convert_blocks_to_data(meta.blocks, meta.codec->bits_per_number());
        char *buffer = storage->current_payload();
        memcpy(buffer, payload, storage->payload_size());

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
            p /= (m.str()+".bmp");
            sync.release();
            char *frame = storage->pop_frame();
            write_frame_data(reinterpret_cast<uint8_t*>(frame), meta.frame_width, meta.frame_height, p);
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
    test_pop_frame();
    return 0;
}