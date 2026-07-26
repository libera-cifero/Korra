#include "timer.hpp"
#include <queue>
#include <vector>
#include <mutex>

using namespace std;
class ip_boxer {
private:
    int _max_sending_size, _timeout;
    queue<char> _package_send_pieces;
    queue<char> _package_receive_pieces;
    mutex _send_mutex;

    timer *_timer;
    char *_pop_data_to_boxing();
    void _setup_timer();
public:
    ip_boxer(int max_sending_size, int timeout);

    //Put package to send_queue and push pices from send_queue to frame_payloads_buffer if queue is overfilled
    void box(char *ip_package, vector<char*> &frame_payloads_buffer);

    //Put decoded_data to receive_queue and set received packages to buffer if packages are built
    void unbox(char *decoded_data, vector<char*> &packages_buffer);

    //If send_queue is not overfilled, but time to boxing is up
    void on_timeout(char *payload_to_send);

    ~ip_boxer();
};