#include "data_boxer/data/korra_data.hpp"
#include "data_boxer/data/piecable_data.hpp"
#include "video_encoder/payload_storage.hpp"
#include <deque>

using namespace std;

class data_boxer {
private:
    //count of abstract korra_data that frame contains. It is first 2 bytes in frame
    uint16_t _data_frame_counter;

    payload_storage *_storage;
    
    void _put_conitinioused_data(korra_data *data, int total_size);

    char *_box_case_A(korra_data *data, int total_size, int free_count);
    char *_box_case_B(korra_data *data, int total_size, int free_count);
    char *_box_case_C(piecable_data *data, int total_size, int free_count);
public:
    data_boxer(payload_storage *storage);
    
    //Convert data to bytes and check, free space count in frame is greater than zero or not.
    void box(korra_data *data);

    ~data_boxer();
};