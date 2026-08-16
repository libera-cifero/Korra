#pragma once
#include <cstdint>
#include <string>
using namespace std;
class tun {
private:
    string _ip;
    uint8_t _subnet_mask;
public:
    tun(string &ip, uint8_t subnet_mask);
    virtual char *read() = 0;
    virtual void write(char *ip_package) = 0;
    string ip();
    string ip_with_mask();
    uint8_t subnet_mask();
    virtual ~tun() = default;
};