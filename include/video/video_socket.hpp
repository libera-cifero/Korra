#pragma once
#include "video_codec/video_codec.hpp"
#include "pipe/video_pipe_in.hpp"
#include "pipe/video_pipe_out.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

using namespace std;
enum net_protocol : char {
    tcp, 
    udp
};

struct ip_endpoint {
    string address;
    uint16_t port;
    net_protocol protocol;
};

struct video_socket_config {
    video_codec *encoder;
    video_pipe_in *pipe_in;
    video_pipe_out *pipe_out;
    ip_endpoint socket;
};

class video_connection {
protected:
    video_socket_config *_config;
public:
    video_connection(video_socket_config*);
    //Read bytes and return readed bytes
    virtual int recieve(void *bytes, size_t max_length) = 0;
    //Write bytes
    virtual void send(void *bytes) = 0;
    ip_endpoint endpoint();
    virtual bool is_alive() = 0;
    //close socket
    ~video_connection();
};

class video_listener {
protected:
    video_socket_config *_config;
public:
    video_listener(video_socket_config*);
    virtual video_connection *accept() = 0;
    ~video_listener();
};