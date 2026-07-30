#pragma once
#include <cstdint>
class identifiable {
private:
    int _generate_random_id();
protected:
    int $id;
    uint16_t $data_size;
public:
    identifiable();
    identifiable(int id, uint16_t size);
    identifiable(uint16_t size);
    int id();
    uint16_t data_size();
};