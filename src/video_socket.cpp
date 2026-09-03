#include "video_socket.hpp"
#include "data_boxer/data/ip_data.hpp"
#include "data_boxer/data/korra_data.hpp"
#include "lib/log.hpp"
#include <format>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

using namespace spdlog;

video_socket::video_socket(video_socket_settings &settings)
{
    _stream_in = settings.stream_in;
    _stream_out = settings.stream_out;
    _tun = settings.tunnel;
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

static std::string format_ip_header(ip_header* header) {
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
           << "Length: " << ntohs(h->total_length) << " | "
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

void video_socket::_run_writer(){
    _stream_out->run();
    string func_prefix = get_method_prefix("video_socket._run_writer");
    _writer_thread = thread([&](){
        while(_is_running){
            bool is_debug = get_level() == level::debug;
            info("{}: reading an IP package from tun {}...", func_prefix, _tun -> name());
            char *ip_package = _tun->read();
            ip_data *ip = new ip_data(ip_package);
            info("{}: package readed from tun {} is readed!", func_prefix, _tun->name());
            if(is_debug){
                debug("{}: {}", func_prefix, format_ip_header(ip->header()));
            }
            info("{}: package is readed!", func_prefix);
            info("{}: writing to stream_out...", func_prefix);
            _stream_out->write(ip);
            info("{}: package was wroten to stream_out!", func_prefix);

            if(is_debug) debug("{}: ip_data is destructing...", func_prefix);
            delete ip;
            if(is_debug) debug("{}: ip_data is destructed!", func_prefix);
        }
    });
}

void video_socket::_run_reader(){
    _reader_thread = thread([&](){
        vector<korra_data*> buffer;
        string func_prefix = format("[THREAD {}] video_socket._run_reader", this_thread::get_id());
        
        while(_is_running){
            bool is_debug = get_level() == level::debug;
            info("{}: reading a korra_data from stream_in to buffer...", func_prefix);
            _stream_in->read(buffer);
            for(int i = 0; i < buffer.size(); i++){
                korra_data *data = buffer[i];
                if(auto ip = dynamic_cast<ip_data*>(data)){
                    info("{}: writing ip_data to tun {}...", func_prefix, _tun->name());
                    if(get_level() == level::debug)
                        debug("{}: {}", func_prefix, format_ip_header(ip->header()));
                    
                    _tun->write(ip->ip_package());
                    info("{}: ip_data is wroten!", func_prefix);
                }
                if(data != nullptr) delete data;
            }
            buffer.clear();
        }
    });
}

void video_socket::run(){
    _is_running = true;
    _run_writer();
    _run_reader();
}

tun *video_socket::get_tun() { return _tun; }
video_stream_in *video_socket::stream_in(){ return _stream_in; }
video_stream_out *video_socket::stream_out(){ return _stream_out; }

video_socket::~video_socket(){
    _is_running = false;
    _reader_thread.join();
    _writer_thread.join();
    delete _tun;
    delete _stream_in;
    delete _stream_out;
}