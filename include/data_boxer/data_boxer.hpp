#include "data_boxer/data/korra_data.hpp"
#include "video_codec/payload_storage.hpp"

using namespace std;

class data_boxer {
private:
    //count of abstract korra_data that frame contains. It is first 2 bytes in frame
    uint16_t _data_frame_counter;

    payload_storage *_storage;
    char *_current_payload;
    
    void _increment_data_count();
    void _begin_new_payload();
    char *_put_conitinioused_data(korra_data *data);
    int _get_free_space();

    bool _is_case_A(korra_data *data);
    bool _is_case_B(korra_data *data); //if case isn't case_A, case_B, then case_C

    void _box_case_A(korra_data *data);
    void _box_case_B(korra_data *data);
    void _box_case_C(korra_data *data);
public:
    data_boxer(payload_storage *storage);
    
    //Convert data to bytes and check, free space count in frame is greater than zero or not.
    void box(korra_data *data);

    ~data_boxer();
};