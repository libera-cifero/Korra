#pragma once
#include "korra_data.hpp"
#include <cstdint>
class ip_data : public korra_data {
private:
    char *_ip_package;
public:
    ip_data();
    ip_data(char *ip_package);

    char *ip_package();

    bool is_splittable() override;

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_bytes() override;
    void from_bytes(char *bytes) override;

    ~ip_data();
};