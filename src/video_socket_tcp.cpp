#include "video/video_socket_tcp.hpp"
#include "video/encoder/provider/len_reader/data_length_reader.hpp"
#include "video/encoder/video_encoder.hpp"
#include <boost/asio.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>

video_listener_tcp::video_listener_tcp(video_socket_config *config) : video_listener(config){
    _acceptor = new tcp::acceptor(_io);

    auto socket = _config->socket;

    tcp::endpoint endpoint(ip::make_address(socket.address), socket.port);
    
    _acceptor->open(endpoint.protocol());
    _acceptor->set_option(tcp::acceptor::reuse_address(true));
    _acceptor->bind(endpoint);
    _acceptor->listen();
}

video_connection* video_listener_tcp::accept(){
    auto client = new tcp::socket(_io);
    _acceptor->accept(*client);
    return new video_connection_tcp(client, _config);
}

video_listener_tcp::~video_listener_tcp(){
    _acceptor->close();
    delete _acceptor;
}

video_connection_tcp::video_connection_tcp(tcp_socket *socket, video_socket_config *config) : video_connection(config) {
    _socket = socket;
}

bool video_connection_tcp::is_alive() { return _socket->is_open(); }

uint16_t video_connection_tcp::_read_package_length(void *data){
    uint16_t length;
    memcpy(&length, (char*)data + 2, 2);
    return length;
}

int video_connection_tcp::recieve(void *bytes, size_t max_length) {
    vector<video_frame> frames;
    data_length_reader *len_reader = _config->encoder->config()->provider->length_reader();
    _config->encoder->encode_package(bytes);
}

void video_connection_tcp::send(void *bytes){
    size_t package_length = _read_package_length(bytes);
    int frame_count;
    vector<video_frame> *frames_ptr = _config->encoder->encode_package(bytes);
    auto frames = *frames_ptr;
    for(int index = 0; index < frame_count; index++){
        auto frame = frames[index];
        uint8_t *data = reinterpret_cast<uint8_t*>(frame.data);
        _config->pipe_out->write(data, frame.size);
    }
    
    delete frames_ptr;
    //boost::asio::write(_socket, boost::asio::buffer((char*)bytes, 2));
}

video_connection_tcp::~video_connection_tcp() {
    _socket->close();
    delete _socket;   
}