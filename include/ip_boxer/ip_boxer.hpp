#include "timer.hpp"
#include "data/korra_data.hpp"
#include <deque>
#include <functional>
#include <vector>
#include <mutex>

using namespace std;
class ip_boxer {
private:
    int _max_sending_size, _timeout;
    vector<function<void(char*)>> _timeout_events;
    deque<char> _package_send_pieces;
    deque<char> _package_receive_pieces;
    mutex _send_mutex;

    timer *_timer;
    char *_pop_bytes_for_boxing();
    void _setup_timer();

    template<typename T>
    void _clear_list(vector<T*> &list, function<void(T*)> deleter);
public:
    ip_boxer(int max_sending_size, int timeout);

    //Put package to send_queue and push pices from send_queue to frame_payloads_buffer if queue is overfilled
    void box(korra_data *data, vector<char*> &frame_payloads_buffer);

    //Put decoded_data to receive_queue and set received packages to buffer if packages are built
    void unbox(char *decoded_data, vector<korra_data*> &data_buffer);

    //If send_queue is not overfilled, but time to boxing is up
    void on_timeout(function<void(char*)> event);

    ~ip_boxer();
};