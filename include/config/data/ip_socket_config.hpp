#include <cstdint>
#include <string>

enum ip_protocol : unsigned char {
    TCP,
    UDP
};

struct ip_socket_config {
    ip_protocol protocol;
    std::string address;
    uint16_t port;
};