#include <cstdint>
#include <semaphore>
#include <thread>

using namespace std;
class clock_generator{
private:
    binary_semaphore *_ready_request, *_ready_response, *_frame_request, *_breaker;
    uint32_t _timeout, _ready_gap_timeout;
    bool _is_running;
    thread *_clock_thread;
public:
    clock_generator(uint32_t timeout, uint32_t ready_gap_timeout);

    int timeout();
    int ready_gap_timeout();

    binary_semaphore *ready_request();
    binary_semaphore *ready_response();
    binary_semaphore *frame_request();

    void launch();

    ~clock_generator();
};