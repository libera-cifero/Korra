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
    void __init_properties(string name, string &ip, uint8_t &subnet_mask) override;
public:
    linux_tun(string &tun_name);
    int mtu() override;
    ~linux_tun();
};