#include "config/parser/parser.hpp"
#include "config/parser_factory/static_parser_factory.hpp"
#include "io.hpp"
#include "video_socket.hpp"
#include "random_data.hpp"
#include "test.hpp"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
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

video_socket *read_socket_from_file(path &file_path){
    path p = DATA_PATH / "config" / "socket.json";
    ifstream file(p);
    stringstream stream;
    stream << file.rdbuf();
    string config_str = stream.str();
    file.close();
    json config = json::parse(config_str);
    video_socket *socket = socket_parser->parse(config);
    return socket;
}

void test_io(){
    const char *test_name = "video_socket_test.test_io";
    printInfo(test_name);

    path socket_A_path = DATA_PATH / "config" / "socket_A.json";
    path socket_B_path = DATA_PATH / "config" / "socket_B.json";

    video_socket *video_socket_A = read_socket_from_file(socket_A_path);
    video_socket *video_socket_B = read_socket_from_file(socket_B_path);

    tcp::socket *tcp_socket_A = make_tcp_socket(video_socket_A->get_tun()->ip(), 1234);
    tcp::socket *tcp_socket_B = make_tcp_socket(video_socket_B->get_tun()->ip(), 1234);

    path seed_path = DATA_SECRET_PATH / "video_socket_test.test_read_write.seed";
    vector<int> sizes={ 100, 4000, 2349, 42400, 1, 2, 3, 400, 10, 900 };
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

int main(){
    test_io();
    return 0;
}