#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "data_boxer/timer.hpp"
#include "video_encoder/payload_storage.hpp"
#include <deque>
#include <functional>

using namespace std;

class data_boxer {
private:
    //count of abstract korra_data that frame contains. It is first 2 bytes in frame
    uint16_t _data_frame_counter;
    deque<char> _payload_buffer;
    int _payload_size_per_frame;
    vector<function<void(char*)>> _events;
    timer *_timer;

    int _last_frame_index();
    int _free_bytes_count();
    void _increment_data_frame_counter();
    void _begin_new_frame();
    
    void _put_conitinioused_data(korra_data *data, int total_size);

    char *_box_case_A(korra_data *data, int total_size, int free_count);
    char *_box_case_B(korra_data *data, int total_size, int free_count);
    char *_box_case_C(piecable_data *data, int total_size, int free_count);
public:
    data_boxer(int payload_size_per_frame);

    char *pop_frame_payload();
    
    //Convert data to bytes and check, free space count in frame is greater than zero or not.
    char* box(korra_data *data);

    void box(korra_data *data, payload_storage *storage);

    void on_timeout(function<void(char*)> event);

    ~data_boxer();
};