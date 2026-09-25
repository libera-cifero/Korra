#include "lib/data/ip_data.hpp"
#include "lib/data/piecable_data.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#ifdef _WIN32
  #include <winsock2.h>
#else
  #include <arpa/inet.h>
#endif

ip_data::ip_data() : piecable_data() {
    _header = nullptr;
}

ip_data::ip_data(uint16_t package_size){
    init_buffer(package_size);
    memcpy(__buffer + 2, &__buffer_size, 2);
    _header = nullptr;
}

ip_data::ip_data(char *ip_package){
    int buffer_size = size(ip_package);
    init_buffer(ip_package, buffer_size);
    _header = _parse_ip_header();
}

ipv4_header *ip_data::_parse_ipv4_header(){
    ipv4_header* header = new ipv4_header;
    const uint8_t* p = reinterpret_cast<uint8_t*>(__buffer);
    
    uint8_t version_ihl = p[0];
    header->version = IPv4;
    header->ihl = (version_ihl & 0x0F);
    
    header->dscp_ecn = p[1];
    header->total_length = (p[2] << 8) | p[3];
    header->identification = (p[4] << 8) | p[5];
    header->flags_fragment_offset = (p[6] << 8) | p[7];
    header->ttl = p[8];
    header->protocol = p[9];
    header->header_checksum = (p[10] << 8) | p[11];
    header->src_ip = (p[12] << 24) | (p[13] << 16) | (p[14] << 8) | p[15];
    header->dst_ip = (p[16] << 24) | (p[17] << 16) | (p[18] << 8) | p[19];
    
    return header;
}

ipv6_header *ip_data::_parse_ipv6_header(){
    ipv6_header* header = new ipv6_header();
    const uint8_t* p = reinterpret_cast<uint8_t*>(__buffer);
    
    uint8_t version_traffic = p[0];
    header->version = IPv6;
    header->traffic_class = ((version_traffic & 0x0F) << 4) | ((p[1] >> 4) & 0x0F);
    header->flow_label = ((p[1] & 0x0F) << 16) | (p[2] << 8) | p[3];
    
    header->payload_length = (p[4] << 8) | p[5];
    header->next_header = p[6];
    header->hop_limit = p[7];
    
    memcpy(header->src_ip, &p[8], 16);
    memcpy(header->dst_ip, &p[24], 16);
    
    return header;
}

ip_header *ip_data::_parse_ip_header(){
    uint8_t version = (__buffer[0] >> 4) & 0x0F;
    
    if (version == IPv4) return _parse_ipv4_header(); 
    else if (version == IPv6) return _parse_ipv6_header();
    
    return nullptr;

}

ip_header *ip_data::header(){
    return _header;
}

uint16_t ip_data::type() { return 0; }

int ip_data::size(){ return piecable_data::size(); }

int ip_data::size(char *raw_bytes){
    uint16_t size = 0;
    memcpy(&size, raw_bytes + 2, 2);
    return ntohs(size);
}

char *ip_data::ip_package() { 
    return __buffer;
}

char *ip_data::to_payload_bytes(char *buffer) {
    if(__buffer == nullptr) throw std::runtime_error("IP package is not defined!");
    memcpy(buffer, __buffer, size());
    return buffer + size();
}

char *ip_data::from_bytes(char *bytes) {
    bytes = korra_data::from_bytes(bytes);
    uint16_t length = size(bytes);

    if(__buffer != nullptr) delete [] __buffer;
    
    init_buffer(length);

    memcpy(__buffer, bytes, length);
    _header = _parse_ip_header();
    return bytes + size();
}

ip_data::~ip_data() { 
    delete [] __buffer;
    if(_header != nullptr) delete _header;
}