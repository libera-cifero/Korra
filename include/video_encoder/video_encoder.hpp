#include "video_encoder/frame_encoder/frame_encoder.hpp"
#include "video_encoder/payload_storage.hpp"
#include "clock_generator.hpp"

class video_encoder {
private:
    clock_generator *_clock_generator;   
    frame_encoder *_frame_encoder;
    payload_storage *_payload_storage;
public:
    video_encoder(frame_encoder *encoder, int fps, int gap_timeout);
    void launch();
    payload_storage *storage();
    ~video_encoder();
};