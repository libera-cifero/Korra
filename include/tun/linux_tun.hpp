#include "tun.hpp"
#include <cstdint>
#include <string>
using namespace std;

class linux_tun : public tun {
private:
    int _file_descriptor;
    string _tun_name;
public:
    linux_tun(string &tun_name, string &ip, uint8_t subnet_mask);
    char *read() override;
    void write(char *ip_package) override;
    ~linux_tun();
};