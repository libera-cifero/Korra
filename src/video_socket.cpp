#include "video_socket.hpp"
video_connection::video_connection(video_socket_config *config){ _config = config; }
ip_endpoint video_connection::endpoint(){ return _config->socket; }
video_connection::~video_connection(){
    delete _config;
}

video_listener::video_listener(video_socket_config *config){
    _config = config;
}
video_listener::~video_listener(){
    delete _config;
}