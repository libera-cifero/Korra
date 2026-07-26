#include "video/video_socket_tcp.hpp"
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

}

void video_connection_tcp::send(void *bytes){

}

video_connection_tcp::~video_connection_tcp() {
    _socket->close();
    delete _socket;   
}