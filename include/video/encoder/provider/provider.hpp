#pragma once
#include <cstddef>

using namespace std;
class provider {
public:    
    virtual size_t frame_size() = 0;
    virtual size_t payload_size() = 0;

    virtual char* to_payload(char* frame) = 0;
    virtual char* to_frame(char* data) = 0;
    virtual ~provider() {};
};