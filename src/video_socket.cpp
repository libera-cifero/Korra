#include "video_socket.hpp"
#include "lib/data/ip_data.hpp"
#include "lib/data/korra_data.hpp"
#include "event/event.hpp"
#include "lib/log.hpp"
#include "event/async_read_event.hpp"
#include "event/event_loop.hpp"
#include "event/timer_event.hpp"
#include <functional>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <boost/asio.hpp>
#include <vector>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

using namespace spdlog;

video_socket::video_socket(video_socket_settings &settings)
{
    _stream_in = settings.stream_in;
    _stream_out = settings.stream_out;
    _tun = settings.tunnel;
    _setup_event_loop(settings);
}

void video_socket::_setup_event_loop(video_socket_settings &settings){
    _loop = new event_loop(100);

    auto tun_read_event = new async_read_event("tun_read_event", [&]() -> char* { return _tun->read(); });
    auto pipe_in_read_event = new async_read_event("pipe_in_read_event", [&]() -> char* { return _stream_in->read_frame(); });
    auto timer = new timer_event(1000 / settings.stream_in->config().fps);

    _loop->observe(tun_read_event);
    _loop->observe(pipe_in_read_event);
    _loop->observe(timer);
    
    _loop->subscribe(tun_read_event->name(), [&](event_args *res){ _on_tun_read_event(static_cast<async_read_event_args*>(res)); });
    _loop->subscribe(pipe_in_read_event->name(), [&](event_args *res){ _on_pipe_in_read_event(static_cast<async_read_event_args*>(res)); });
    _loop->subscribe(timer->name(), [&](event_args *res) { _on_timer_event(static_cast<timer_event_args*>(res)); });
}

void video_socket::_on_tun_read_event(async_read_event_args *args){
    string func_prefix = get_method_prefix("video_socket._on_tun_read_event");
    bool is_debug = get_level() == level::debug;
    if(is_debug) debug("{} ip_data forming...", func_prefix);
    ip_data *ip = new ip_data(args->data);
    if(is_debug) debug("{} ip data was formed!", func_prefix);
    info("{} package readed from tun {} is readed!", func_prefix, _tun->name());
    if(is_debug) debug("{} {}", func_prefix, format_ip_header(ip->header()));
    info("{} package is readed!", func_prefix);
    info("{} writing to stream_out...", func_prefix);
    _stream_out->write(ip);
    info("{} package was wroten to stream_out!", func_prefix);

    if(is_debug) debug("{} ip_data is destructing...", func_prefix);
    delete ip;
    if(is_debug) debug("{} ip_data is destructed!", func_prefix);
}

void video_socket::_on_pipe_in_read_event(async_read_event_args *args){
    vector<korra_data*> buffer;
    string func_prefix = get_method_prefix("video_socket._on_pipe_in_read_event");
    
    _stream_in->unbox_frame(args->data, buffer);
    if(buffer.size() > 0)
        info("{} reading a korra_data from stream_in to buffer...", func_prefix);

    for(int i = 0; i < buffer.size(); i++){
        korra_data *data = buffer[i];
        if(auto ip = dynamic_cast<ip_data*>(data)){
            string header_str = format_ip_header(ip->header());
            debug("{} received ip_data with header {}", func_prefix, header_str);
            info("{} writing ip_data to tun {}...", func_prefix, _tun->name());
            if(get_level() == level::debug)
                debug("{} {}", func_prefix, format_ip_header(ip->header()));
            
            _tun->write(ip->ip_package());
            info("{}: ip_data is wroten!", func_prefix);
        }
        if(data != nullptr) delete data;
    }

    delete [] args->data;
}

void video_socket::_on_timer_event(timer_event_args *args){
    _stream_out->send_current_frame();
}

void video_socket::run(){
    _loop->run();
}

void video_socket::stop(){
    _loop->stop();
}

tun *video_socket::get_tun() { return _tun; }
video_stream_in *video_socket::stream_in(){ return _stream_in; }
video_stream_out *video_socket::stream_out(){ return _stream_out; }

video_socket::~video_socket(){
    auto prefix = get_method_prefix("video_socket.~video_socket");
    spdlog::debug("{} destructing...", prefix);
    delete _loop;
    delete _tun;
    delete _stream_in;
    delete _stream_out;
    spdlog::debug("{} video_socket destructed!", prefix);
}