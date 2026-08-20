#include "video_socket.hpp"
#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "tun/tun_builder.hpp"
#include <cstdint>
#include <mutex>
#include <thread>

video_socket::video_socket(video_socket_settings &settings)
{
    _boxer = settings.boxer;
    _unboxer = settings.unboxer;
    _codec = settings.codec;
    _pipe_in = settings.pipe_in;
    _pipe_out = settings.pipe_out;
    _tun = build_tun(settings.tun);
}

void video_socket::_run_codec_handler(){
    _codec->launch();
    _thread_codec = thread([&](){
        auto storage = _codec->storage();
        while(_is_running){
            _frame_to_write = storage->pop_frame();
            lock_guard<mutex> lock(_frame_to_write_mutex);
            _frame_updated = true;
        }
    });
}

void video_socket::_run_writer(){
    _thread_writer = thread([&](){
        int frame_size = _codec->frame_size();
        while(_is_running){
            char *ip_package = _tun->read();
            ip_data *ip = new ip_data(ip_package);
            _boxer -> box(ip);
            if(_frame_updated) {
                _pipe_out->write(reinterpret_cast<uint8_t*>(_frame_to_write), frame_size);
                lock_guard<mutex> lock(_frame_to_write_mutex);
                delete [] _frame_to_write;
                _frame_to_write = nullptr;
                _frame_updated = false;
            }

            delete ip;
        }
    });
}

void video_socket::_run_reader(){
    _thread_reader = thread([&](){
        int frame_size = _codec->frame_size(), payload_size = _codec->payload_size();
        korra_data *received_data = nullptr;

        while(_is_running){
            auto received_frame = _pipe_in->read(frame_size);
            char *bytes = _codec->decode(reinterpret_cast<char*>(received_frame));
            _unboxer->put_payload(bytes, payload_size);
            do{
                received_data = _unboxer->unbox();
                if(auto ip = dynamic_cast<ip_data*>(received_data)){
                    _tun->write(ip->ip_package());
                }
                delete received_data;
            }
            while(received_data != nullptr);
        }
    });
}

void video_socket::run(){
    _is_running = true;
    _run_codec_handler();
    _run_writer();
    _run_reader();
}

video_socket::~video_socket(){
    _is_running = false;
    _thread_codec.join();
    _thread_writer.join();
    _thread_reader.join();
    if(_frame_to_write != nullptr)
        delete [] _frame_to_write;
    delete _tun;
    delete _pipe_in;
    delete _pipe_out;
    delete _codec;
    delete _boxer;
    delete _unboxer;
}