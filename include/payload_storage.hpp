#include "video/encoder/video_encoder.hpp"
#include <mutex>
#include <deque>

using namespace std;
class payload_storage {
private:
    video_encoder *_encoder;
    mutex *_ready_request, *_ready_response, *_root_request;
    
    deque<char*> _payloads;
    char *_frame;
public:
    payload_storage(video_encoder *encoder, mutex *ready_request, mutex *ready_response, mutex *root_request);

    int payload_size();
    int frame_size();
    int frame_count();

    char *get_frame();
    void put_data(char *data, int data_size);

    void begin_new_payload();
};