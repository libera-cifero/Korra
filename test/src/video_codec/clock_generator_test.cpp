#include "video_codec/clock_generator.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

double get_delta_millis(timespec t0, timespec t1){
    return ((t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9) * 1000.0;
}

int main(){
    clock_generator gen(33);

    gen.launch();

    for(int i = 0; i < 50; i++){
        cout<<i<<endl;
        timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        gen.signals()->ready_request()->acquire();
        this_thread::sleep_for(std::chrono::milliseconds(10));
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double millis = get_delta_millis(t0, t1);
        cout << "TIME: "<<millis <<"ms Done!"<<endl;
    }

    return 0;
}