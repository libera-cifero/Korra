#pragma once
#include "tun.hpp"
#include <cstdint>
#include <string>
using namespace std;

class linux_tun : public tun {
private:
    int _file_descriptor;
    int _mtu;

    void _open_tun();
protected:
    int __read(char *buffer, int count) override;
    int __write(char *buffer, int count) override;
    void __init_properties(string ip, string *name, uint8_t *subnet_mask) override;
public:
    linux_tun(string &ip);
    int mtu() override;
    ~linux_tun();
};