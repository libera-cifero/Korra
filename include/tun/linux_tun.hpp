#pragma once
#include "tun.hpp"
#include <cstdint>
#include <string>
using namespace std;

class linux_tun : public tun {
private:
    int _file_descriptor;
    int _mtu;
protected:
    int __read(char *buffer, int count) override;
    int __write(char *buffer, int count) override;
public:
    linux_tun(string &tun_name, string &ip, uint8_t subnet_mask);
    int mtu() override;
    ~linux_tun();
};