#include <cstdint>

struct service_section {
    uint16_t section_size;
    
};

struct payload_header {
    uint32_t frame_number;
    char prev_payload_hash[32];

    uint16_t header_size;
    uint32_t payload_size;

    service_section service;
};