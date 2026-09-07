#pragma once
#include "tun.hpp"
#include <cstdint>
#include <string>
using namespace std;

class linux_tun : public tun {
private:
    int _file_descriptor;
    int _mtu;
    string _tun_name;
    char *_read_buffer;
    int _readed_count;
    int _package_read_count;
    bool _is_header_reading;

    char *_current_read_buffer();
public:
    linux_tun(string &tun_name, string &ip, uint8_t subnet_mask);
    char *read() override;
    void write(char *ip_package) override;
    int mtu() override;
    ~linux_tun();
};