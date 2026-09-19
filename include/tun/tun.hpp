#pragma once
#include <cstdint>
#include <string>
using namespace std;
class tun {
private:
    string _name;
    string _ip;
    int _ip_val;
    uint8_t _subnet_mask;
    bool _is_header_reading;
    int _readed_count;
    int _package_read_count;

    char *_read_buffer;
    char *_current_read_buffer();
    bool _must_transmit();
protected:
    virtual int __read(char *buffer, int count) = 0;
    virtual int __write(char *buffer, int count) = 0;
    virtual void __init_properties(string ip, string *name, uint8_t *subnet_mask) = 0;
public:
    tun(string &ip);
    void init();
    char *read();
    void write(char *ip_package);
    virtual int mtu() = 0;
    string ip();
    string ip_with_mask();
    uint8_t subnet_mask();
    string name();
    virtual ~tun();
};