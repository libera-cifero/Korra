#include "encoder/video_encoder.hpp"
#include <cstddef>
#include <vector>
class estimator {
public:
    virtual float estimate(std::vector<video_frame>& frames, size_t payload_bytes) = 0;
};