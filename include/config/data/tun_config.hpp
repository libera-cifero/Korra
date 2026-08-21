#include <cstdint>
#include <string>
struct tun_config {
    std::string name;
    std::string ip;
    uint8_t mask;
};