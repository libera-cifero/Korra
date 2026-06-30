#pragma once
#include <cstdint>
#include <stdexcept>
class status_error : public std::runtime_error {
private:
    uint32_t _status;
public:
    status_error(const char *str, int status) : std::runtime_error(str){
        _status = status;
    }
    uint32_t status();
};