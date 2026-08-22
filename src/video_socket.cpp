#include "video_socket.hpp"
#include "config/data/video_config.hpp"
#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

video_socket::video_socket(video_socket_settings &settings)
{
    _stream_in = settings.stream_in;
    _stream_out = settings.stream_out;
    _tun = settings.tun;
}

void video_socket::_run_writer(){
    _stream_out->run();
    _writer_thread = thread([&](){
        while(_is_running){
            char *ip_package = _tun->read();
            ip_data *ip = new ip_data(ip_package);
            _stream_out->write(ip);
            delete ip;
        }
    });
}

void video_socket::_run_reader(){
    _reader_thread = thread([&](){
        vector<korra_data*> buffer;

        while(_is_running){
            _stream_in->read(buffer);
            for(int i = 0; i < buffer.size(); i++){
                korra_data *data = buffer[i];
                if(auto ip = dynamic_cast<ip_data*>(data)){
                    _tun->write(ip->ip_package());
                }
                delete data;
            }
            buffer.clear();
        }
    });
}

void video_socket::run(){
    _is_running = true;
    _run_codec_handler();
    _run_writer();
    _run_reader();
}

tun *video_socket::tun() { return _tun; }

video_socket::~video_socket(){
    _is_running = false;
    _reader_thread.join();
    _writer_thread.join();
    delete _tun;
    delete _stream_in;
    delete _stream_out;
}