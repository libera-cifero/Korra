#pragma once
#include "data/korra_data.hpp"
#include <vector>
#include <functional>

using namespace std;

class data_boxer {
public:
    //Put package to send_queue and push pices from send_queue to frame_payloads_buffer if queue is overfilled
    virtual void box(korra_data *data, vector<char*> &frame_payloads_buffer) = 0;

    //Put decoded_data to receive_queue and set received packages to buffer if packages are built
    virtual void unbox(char *decoded_data, vector<korra_data*> &data_buffer) = 0;

    //If send_queue is not overfilled, but time to boxing is up
    virtual void on_timeout(function<void(char*)> event) = 0;

    virtual ~data_boxer(){};
};