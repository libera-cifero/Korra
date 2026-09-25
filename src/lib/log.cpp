#include "lib/log.hpp"
#include "lib/data/ip_data.hpp"
#include <cstdint>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <boost/asio.hpp>
#include <format>
#include <thread>

string get_method_prefix(string method_name){
    return format("[THREAD {}] {}:", this_thread::get_id(), method_name);
}

bool is_debug_level(){
    return spdlog::get_level() == spdlog::level::debug;
}

string get_byte_str(void *bytes){
    if(bytes == nullptr || bytes == NULL)
        return "NULL";
    
    uint8_t *data = reinterpret_cast<uint8_t*>(bytes);
    string str = "";
    for(int i = 0; i < BYTE_STR_DISPLAY_COUNT; i++){
        if(i>0) str+=" ";
        str+=to_string(data[i]);
    }

    str+="...";
    return str;
}

static std::string ipv4_to_string(uint32_t ip) {
    boost::asio::ip::address_v4 addr(ip);
    return addr.to_string();
}

static std::string ipv6_to_string(const uint8_t* ip) {
    boost::asio::ip::address_v6::bytes_type bytes;
    std::copy(ip, ip + 16, bytes.data());
    boost::asio::ip::address_v6 addr(bytes);
    return addr.to_string();
}

static std::string get_protocol_name(uint8_t protocol) {
    switch (protocol) {
        case 1: return "ICMP";
        case 6: return "TCP";
        case 17: return "UDP";
        case 41: return "IPv6";
        case 47: return "GRE";
        case 50: return "ESP";
        case 51: return "AH";
        default: return "Unknown (" + std::to_string(protocol) + ")";
    }
}

std::string format_ip_header(ip_header* header) {
    std::stringstream ss;
    
    if (header->version == IPv4) {
        ipv4_header* h = static_cast<ipv4_header*>(header);
        
        uint16_t flags_offset = ntohs(h->flags_fragment_offset);
        uint8_t flags = (flags_offset >> 13) & 0x07;
        uint16_t fragment_offset = flags_offset & 0x1FFF;
        
        std::string flags_str = "";
        if (flags & 0x04) flags_str += "Reserved ";
        if (flags & 0x02) flags_str += "DF ";
        if (flags & 0x01) flags_str += "MF";
        
        ss << "IPv4: " << ipv4_to_string(h->src_ip) << " -> " 
           << ipv4_to_string(h->dst_ip) << " | "
           << "Protocol: " << get_protocol_name(h->protocol) << " | "
           << "Length: " << h->total_length << " | "
           << "TTL: " << (int)h->ttl << " | "
           << "Flags: [" << flags_str << "] | "
           << "ID: 0x" << std::hex << ntohs(h->identification);
           
    } else if (header->version == IPv6) {
        ipv6_header* h = static_cast<ipv6_header*>(header);
        
        ss << "IPv6: " << ipv6_to_string(h->src_ip) << " -> " 
           << ipv6_to_string(h->dst_ip) << " | "
           << "Protocol: " << get_protocol_name(h->next_header) << " | "
           << "Payload: " << ntohs(h->payload_length) << " | "
           << "Hop Limit: " << (int)h->hop_limit << " | "
           << "Flow Label: 0x" << std::hex << h->flow_label;
    }
    
    return ss.str();
}