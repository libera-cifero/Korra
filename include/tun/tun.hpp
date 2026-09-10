#pragma once
#include <cstdint>
#include <string>
using namespace std;
class tun {
private:
    string _name;
    string _ip;
    uint8_t _subnet_mask;
    bool _is_header_reading;
    int _readed_count;
    int _package_read_count;

    char *_read_buffer;
    char *_current_read_buffer();
protected:
    virtual int __read(char *buffer, int count) = 0;
    virtual int __write(char *buffer, int count) = 0;
public:
    tun(string &name,string &ip, uint8_t subnet_mask);
    void init_buffer();
    char *read();
    void write(char *ip_package);
    virtual int mtu() = 0;
    string ip();
    string ip_with_mask();
    uint8_t subnet_mask();
    string name();
    virtual ~tun();
};