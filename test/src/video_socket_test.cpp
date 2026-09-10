#include "config/parser/parser.hpp"
#include "config/parser_factory/static_parser_factory.hpp"
#include "io.hpp"
#include "video_socket.hpp"
#include "random_data.hpp"
#include "test.hpp"
#include <arpa/inet.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <netinet/in.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <boost/asio.hpp>

using namespace filesystem;
using namespace boost::asio::ip;

class timeout_exception : public exception { };

auto *socket_parser = static_parser_factory().build();

tcp::socket *make_tcp_socket(string tun_ip, uint16_t tun_port) {
    boost::asio::io_service service;
    tcp::socket *socket = new tcp::socket(service);
    auto address = boost::asio::ip::address_v4::from_string(tun_ip);
    tcp::endpoint dest_ep(address, tun_port);
    socket->connect(dest_ep);
    return socket;
}

udp::socket *make_udp_socket(string tun_ip){
    boost::asio::io_service service;
    udp::socket *socket = new udp::socket(service);
    auto address_bin = boost::asio::ip::address_v4::from_string(tun_ip).to_uint() + 1;
    auto address = boost::asio::ip::address_v4(address_bin);
    //socket->bind(udp::endpoint(address, 1234));
    return socket;
}

char *receive_with_timeout(tcp::socket *socket, int payload_size, int timeout){
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket->native_handle(), &readfds);
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout * 1000;
    
    int ret = select(socket->native_handle() + 1, &readfds, NULL, NULL, &tv);
    char *buffer = new char[payload_size];
    if(ret > 0){
        boost::system::error_code error;
        size_t bytes = socket->receive(boost::asio::buffer(buffer, payload_size), 0, error);
        return buffer;
    }
    delete [] buffer;

    if(ret == 0) throw timeout_exception();
    else throw runtime_error("Socket is invalid!");
}

char *receive_udp_with_timeout(udp::socket *socket, int payload_size, int timeout){
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket->native_handle(), &readfds);
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout * 1000;
    
    int ret = select(socket->native_handle() + 1, &readfds, NULL, NULL, &tv);
    char *buffer = new char[payload_size];
    if(ret > 0){
        boost::system::error_code error;
        size_t bytes = socket->receive(boost::asio::buffer(buffer, payload_size), 0, error);
        return buffer;
    }
    delete [] buffer;

    if(ret == 0) throw timeout_exception();
    else throw runtime_error("Socket is invalid!");
}

video_socket *read_socket_from_file(path &file_path){
    ifstream file(file_path);
    stringstream stream;
    stream << file.rdbuf();
    string config_str = stream.str();
    file.close();
    json config = json::parse(config_str);
    video_socket *socket = socket_parser->parse(config);
    return socket;
}

void test_write_udp(){
    const char *test_name = "video_socket_test.test_write_udp";
    printInfo(test_name);

    path socket_A_path = DATA_PATH / "config" / "socket_A.json";

    video_socket *video_socket_A = read_socket_from_file(socket_A_path);

    udp::socket *udp_socket_A = make_udp_socket(video_socket_A->get_tun()->ip());

    int udp_socket_native = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in dest{
        .sin_family = AF_INET,
        .sin_port = 9999
    };
    inet_pton(AF_INET, "10.18.193.2", &dest.sin_addr);

    path seed_path = DATA_SECRET_PATH / "video_socket_test.test_read_write.seed";
    thread t([](video_socket *socket){ socket->run(); }, video_socket_A);

    string error_reason;
    bool is_error = false;

    auto dest_ip = boost::asio::ip::address_v4::from_string("10.18.193.2");
    auto dest_ep = udp::endpoint(dest_ip, 1234);
    uint64_t seed = get_seed(seed_path);
    int max_size = video_socket_A->get_tun()->mtu() - 100;
    try{
        for(int i = 0; i < 10000; i++){
            printInfo("%d sending...", i);
            int size = rand() % max_size;
            char *payload = random_array(seed, size, seed);
            int sent_count = sendto(udp_socket_native, payload, size, 0, (struct sockaddr*)&dest, sizeof(dest));
            delete [] payload;
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
    catch(exception &e){
        is_error = true;
        error_reason = e.what();
    }
    video_socket_A->stop();
    t.join();
    delete udp_socket_A;
    delete video_socket_A;

    if(is_error) fail(test_name, error_reason.c_str(), 1);
    printPass(test_name);

}

void test_io_tcp(){
    const char *test_name = "video_socket_test.test_io_tcp";
    printInfo(test_name);

    path socket_A_path = DATA_PATH / "config" / "socket_A.json";
    path socket_B_path = DATA_PATH / "config" / "socket_B.json";

    video_socket *video_socket_A = read_socket_from_file(socket_A_path);
    video_socket *video_socket_B = read_socket_from_file(socket_B_path);

    tcp::socket *tcp_socket_A = make_tcp_socket(video_socket_A->get_tun()->ip(), 1234);
    tcp::socket *tcp_socket_B = make_tcp_socket(video_socket_B->get_tun()->ip(), 1234);

    path seed_path = DATA_SECRET_PATH / "video_socket_test.test_read_write.seed";
    vector<int> sizes = { 100, 4000, 2349, 42400, 1, 2, 3, 400, 10, 900 };
    int len_count = sizes.size();
    auto test_cases = get_random_payloads(seed_path, sizes.data(), len_count);

    video_socket_A->run();
    video_socket_A->run();

    string error_reason;
    bool is_error = false;

    try{
        for(int i = 0; i < len_count; i++){
            auto test_case = test_cases[i];
            char *payload = get<0>(test_case);
            int size = get<1>(test_case);
            printInfo("%d sending...", i);
            tcp_socket_A->write_some(boost::asio::buffer(payload, size));
            printInfo("%d reading...");
            char *received = receive_with_timeout(tcp_socket_B, size, 5000);
            printInfo("%d data readed!");
            is_error = memcmp(payload, received, size) != 0;
            delete [] received;
            if(is_error) throw runtime_error(format("test_cases[{}] with size {} is invalid!", i, size));
        }
    }
    catch(exception &e){
        is_error = true;
        error_reason = e.what();
    }
    delete tcp_socket_A;
    delete tcp_socket_B;
    delete video_socket_A;
    delete video_socket_B;
    free_random_payloads(test_cases, len_count);

    if(is_error) fail(test_name, error_reason.c_str(), 1);
    printPass(test_name);
}

void test_io_udp(){
    const char *test_name = "video_socket_test.test_io_udp";
    printInfo(test_name);
    path socket_A_path = DATA_PATH / "config" / "socket_A.json";
    path socket_B_path = DATA_PATH / "config" / "socket_B.json";

    video_socket *video_socket_A = read_socket_from_file(socket_A_path);
    video_socket *video_socket_B = read_socket_from_file(socket_B_path);

    udp::socket *udp_socket_A = make_udp_socket(video_socket_A->get_tun()->ip());
    udp::socket *udp_socket_B = make_udp_socket(video_socket_B->get_tun()->ip());

    path seed_path = DATA_SECRET_PATH / "video_socket_test.test_read_write.seed";
    vector<int> sizes = { 100, 4000, 2349, 42400, 1, 2, 3, 400, 10, 900 };
    int len_count = sizes.size();
    auto test_cases = get_random_payloads(seed_path, sizes.data(), len_count);

    video_socket_A->run();
    video_socket_A->run();

    string error_reason;
    bool is_error = false;

    try{
        for(int i = 0; i < len_count; i++){
            auto test_case = test_cases[i];
            char *payload = get<0>(test_case);
            int size = get<1>(test_case);
            printInfo("%d sending...", i);
            udp_socket_A->send(boost::asio::buffer(payload, size));
            printInfo("%d reading...");
            char *received = receive_udp_with_timeout(udp_socket_B, size, 5000);
            printInfo("%d data readed!");
            is_error = memcmp(payload, received, size) != 0;
            delete [] received;
            if(is_error) throw runtime_error(format("test_cases[{}] with size {} is invalid!", i, size));
        }
    }
    catch(exception &e){
        is_error = true;
        error_reason = e.what();
    }
    delete udp_socket_A;
    delete udp_socket_B;
    delete video_socket_A;
    delete video_socket_B;
    free_random_payloads(test_cases, len_count);

    if(is_error) fail(test_name, error_reason.c_str(), 1);
    printPass(test_name);
}

int main(){
    spdlog::set_level(spdlog::level::debug);
    test_write_udp();
    //test_io_udp();
    //test_io_tcp();
    return 0;
}