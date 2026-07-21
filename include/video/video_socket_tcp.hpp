#pragma once
#include "video_socket.hpp"
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

using namespace boost::asio;
using ip::tcp;
using tcp_socket = tcp::socket;

class video_listener_tcp : public video_listener {
private:
    io_context _io;
    tcp::acceptor *_acceptor = nullptr;
public:
    video_listener_tcp(video_socket_config*);
    video_connection *accept() override;
    ~video_listener_tcp();
};

class video_connection_tcp : public video_connection {
private:
    tcp::socket *_socket;
    static uint16_t _read_package_length(void *package);
public:
    video_connection_tcp(tcp::socket*, video_socket_config*);
    //Read bytes and return readed bytes
    int recieve(void *bytes, size_t max_length) override;
    //Write bytes
    void send(void *bytes) override;
    bool is_alive() override;
    //close socket
    ~video_connection_tcp();
};