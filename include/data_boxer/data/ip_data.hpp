#pragma once
#include "piecable_data.hpp"
#include <cstdint>

enum ip_version : uint8_t {
    IPv4 = 4,
    IPv6 = 6
};

struct ip_header {
    ip_version version;
    virtual ~ip_header() = default;
};

#pragma pack(1)
struct ipv4_header : public ip_header {
    uint8_t ihl;                   // Internet Header Length
    uint8_t dscp_ecn;              // DSCP (6 bit) and ECN (2 bit)
    uint16_t total_length;         // Total package length (header + data)
    uint16_t identification;       // Package id
    uint16_t flags_fragment_offset; // Flags (3 bits) and fragment offset (13 bits)
    uint8_t ttl;                   // Time To Live
    uint8_t protocol;              // Protocol (TCP=6, UDP=17, ICMP=1 etc)
    uint16_t header_checksum;      // Checksum of header
    uint32_t src_ip;               // IP address of source
    uint32_t dst_ip;               // IP address of destination
};

struct ipv6_header : public ip_header {
    uint8_t traffic_class;         // Traffic Class (DSCP + ECN)
    uint32_t flow_label : 20;      // Flow Label (20 бит)
    uint16_t payload_length;       // Length of data without header
    uint8_t next_header;           // Type of next header (TCP=6, UDP=17, etc.)
    uint8_t hop_limit;             // Equivalent to TTL in IPv4
    uint8_t src_ip[16];            // IP address of source (128 bits)
    uint8_t dst_ip[16];            // IP address of destination (128 бит)
};
#pragma pack()

/*
    Wrapper for IP package

    BYTE's STRUCTURE
    +-----------+--------------+
    | TYPE = 0  | PAYLOAD DATA |
    | 2 bytes   |   N BYTES    |
    +-----------+--------------+
                |  IP PACKAGE  |
                |   N bytes    |
                +--------------+
*/
class ip_data : public piecable_data {
private:
    ip_header *_header;
    ipv4_header *_parse_ipv4_header();
    ipv6_header *_parse_ipv6_header();
    ip_header *_parse_ip_header();
public:
    ip_data();
    ip_data(uint16_t package_size);
    ip_data(char *ip_package);

    ip_header *header();
    char *ip_package();

    int size() override;
    int size(char *bytes) override;

    uint16_t type() override;
    char *to_payload_bytes(char *buffer) override;
    char *from_bytes(char *bytes) override;

    ~ip_data();
};