#include "frame_meta.hpp"
#include "video_encoder/frame_encoder/frame_encoder.hpp"
#include "video_encoder/frame_encoder/provider/mosaic/mosaic_settings.hpp"
#include "video_encoder/frame_encoder/provider/mosaic_provider.hpp"
#include "video_encoder/frame_encoder/provider/provider.hpp"
#include "video_encoder/payload_storage.hpp"
#include "video_encoder/frame_encoder/cipher/none_cipher.hpp"
#include "frame_io.hpp"
#include "test.hpp"
#include "video_encoder/sync_signals.hpp"
#include <cstdint>
#include <semaphore>
#include <string>
using namespace std;

const string base_name = "payload_storage_test";

void test_payload(){

}

void test_pop_frame(){
    const string test_name = base_name + ".test_pop_frame";

    iterate_frame_test_cases(test_name.c_str(), [](const char *test_name, frame_meta m, uint8_t *data, string file){
        provider *p = new mosaic_provider(&m);
        cipher *c = new none_cipher(p->payload_size());
        frame_encoder *encoder = new frame_encoder(p, c);
        sync_signals *signals = new sync_signals;
        payload_storage *storage = new payload_storage(encoder, signals);

        

        delete signals;
        delete storage;
        delete encoder;
    });
    
    printPass(test_name.c_str());
}

int main(){
    return 0;
}