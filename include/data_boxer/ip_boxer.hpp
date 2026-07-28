#include "data_boxer.hpp"
#include "timer.hpp"
#include "data/korra_data.hpp"
#include "checker/data_checker.hpp"
#include <deque>
#include <functional>
#include <vector>
#include <mutex>

using namespace std;
class ip_boxer : data_boxer {
private:
    using iterator = deque<char>::iterator;

    int _max_sending_size, _timeout;
    vector<function<void(char*)>> _timeout_events;
    deque<char> _package_send_pieces;
    deque<char> _package_receive_pieces;

    vector<char> _processed_data;
    uint16_t _data_type;
    vector<data_checker<iterator>*> _checkers;

    mutex _send_mutex;
    timer *_timer;
    
    char *_pop_bytes_for_boxing();
    void _setup_timer();

    template<typename T>
    void _clear_list(vector<T*> &list, function<void(T*)> deleter);
public:
    ip_boxer(int max_sending_size, int timeout);

    //Put package to send_queue and push pices from send_queue to frame_payloads_buffer if queue is overfilled
    void box(korra_data *data, vector<char*> &frame_payloads_buffer) override;

    //Put decoded_data to receive_queue and set received packages to buffer if packages are built
    void unbox(char *decoded_data, vector<korra_data*> &data_buffer) override;

    //If send_queue is not overfilled, but time to boxing is up
    void on_timeout(function<void(char*)> event) override;

    ~ip_boxer();
};