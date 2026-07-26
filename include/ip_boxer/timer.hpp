#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <thread>
#include <vector>
#include <functional>
#include <boost/asio.hpp>
#include <chrono>
#include <mutex>

using namespace std;
using boost::asio::steady_timer;
typedef boost::asio::basic_waitable_timer<std::chrono::steady_clock>* cancellable_clock;
//it launch every subscribed event after timeout_millis milliseconds
class timer {
private:
    int _timeout_millis;
    vector<function<void()>> _events;
    jthread _thread;
    bool _is_running, _is_destroying;
    boost::asio::io_context _io;
    cancellable_clock _cancellable_clock;
    mutex _clock_mutex;

    cancellable_clock _make_clock();
public:
    timer(int timeout_millis);
    int timeout_millis();
    void launch();
    void reset();
    void on_timeout(function<void()> event);
    ~timer();
};