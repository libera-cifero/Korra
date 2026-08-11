#include "config/data/video_config.hpp"
#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data_boxer.hpp"
#include "data_boxer/data_unboxer.hpp"
#include "frame_io.hpp"
#include "io.hpp"
#include "test.hpp"
#include "video_codec/payload_storage.hpp"
#include "video_codec/video_codec.hpp"
#include <cstring>
#include <vector>
#include <random>

char *random_ip_package(uint16_t size) {
    char *package = new char[size];
    memcpy(package + 2, &size, 2);
    std::random_device seed; 
    std::mt19937_64 gen(seed());
    std::uniform_int_distribution<> dis(-127, 128);

    for(int i = 0; i < size; i++) {
        if(i < 2 || i > 3) package[i] = dis(gen);
    }
    
    return package;
}

bool compare_datas(korra_data *a, korra_data *b){
    if(auto a_ip = dynamic_cast<ip_data*>(a)){
        if(auto b_ip = dynamic_cast<ip_data*>(b)){
            if(a_ip->size() == b_ip->size()){
                char *package_a = a_ip->ip_package(), *package_b = b_ip->ip_package();
                for(int i = 0; i < a_ip->size(); i++){
                    if(package_a[i] != package_b[i]) return false;
                }
                return true;
            }
        }
    }
    return false;
}

void test_box_unbox(){
    const char *test_name = "data_boxer_test.test_box_unbox";
    
    vector<tuple<string, video_config, vector<korra_data*>>> test_cases
    { 
        {
            "rgb_palette0.json", 
            {
                .frame_width = 1280,
                .frame_height = 720
            }, 
            {
                new ip_data(random_ip_package(1796)),
                new ip_data(random_ip_package(1797)),
                new ip_data(random_ip_package(1798)),
                new ip_data(random_ip_package(123)),
            }
        },
        {
            "rgb_palette1.json", 
            {
                .frame_width = 1024, 
                .frame_height = 768
            }, 
            {
                new ip_data(random_ip_package(2249)),
                new ip_data(random_ip_package(2250)),
                new ip_data(random_ip_package(2251)),
                new ip_data(random_ip_package(23401)),
                new ip_data(random_ip_package(4512))
            }
        }
    };
    printInfo(test_name);
    for(auto test_case : test_cases) 
    {
        string codec_name = get<0>(test_case);
        string codec_path = DATA_FRAME_CODEC_PATH / to_native_path(codec_name);
        printInfo(codec_name.c_str());
        frame_io io_context;

        auto codec = io_context.read_codec_from_file(codec_path, get<1>(test_case));
        video_codec *vc = new video_codec(codec, 30, 15);
        data_boxer boxer(vc->storage());
        vector<korra_data*> datas = get<2>(test_case);

        for(korra_data *data : datas) boxer.box(data);

        vector<char*> payloads;
        vc->launch();
        char *prev_frame = nullptr;
        while(true){
            char *frame = vc->storage()->pop_frame();
            char *payload = codec->decode(frame);
            delete [] frame;
            if(payload[0] == 0 && payload[1] == 0) break;
            payloads.push_back(payload);
        }
        int payload_size = codec->payload_size();
        delete vc;

        data_unboxer unboxer;
        vector<korra_data*> unboxed_payloads;
        for(auto payload : payloads){
            unboxer.put_payload(payload, payload_size);
            korra_data *unboxed = nullptr;
            do{
                unboxed = unboxer.unbox();
                if(unboxed != nullptr) unboxed_payloads.push_back(unboxed);
            }
            while(unboxed != nullptr);
        }

        if(unboxed_payloads.size() != datas.size()){
            for(int i = 0; i < unboxed_payloads.size(); i++) delete unboxed_payloads[i];
            for(int i = 0; i < datas.size(); i++) delete datas[i];
            io_context.~frame_io();
            fail(test_name, "count of received and sent aren't equal!", 1);
        }
        for(int i = 0; i < datas.size(); i++){
            korra_data *a = datas[i];
            korra_data *b = unboxed_payloads[i];
            if(!compare_datas(a, b)){
                for(int j = 0; j < unboxed_payloads.size(); j++) delete unboxed_payloads[j];
                for(int j = 0; j < datas.size(); j++) delete datas[j];
                io_context.~frame_io();
                fail(test_name, "boxed and unboxed aren't same on index %d!", 1, i);
            }
        }
    }

    printPass(test_name);
}

int main(){
    test_box_unbox();
    return 0;
}