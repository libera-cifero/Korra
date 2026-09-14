#include "data_boxer/data/ip_data.hpp"
#include "tun/linux_tun.hpp"
#include "test.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <format>
#include <filesystem>
#include <netinet/in.h>
#include <semaphore>
#include <string>
#include <sys/socket.h>
#include <thread>

bool is_test_read = false, is_test_write = false;
std::string run_command(const std::string &cmd) {
    std::array<char, 4096> buffer;
    std::string result;

    FILE *pipe = popen(cmd.c_str(), "r");  // "r" = читаем stdout команды
    if (!pipe) {
        throw std::runtime_error("popen() failed");
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    int status = pclose(pipe);
    printInfo("status code: %d", status);
    return result;
}

bool is_tun_made(string &tun_name){
    return system(format("ip link show {} > /dev/null", tun_name).c_str()) == 0;
}

void test_read(){
    is_test_read = true;
    const char *test_name = "linux_tun_test.test_read";
    printInfo(test_name);
    string tun_name = "korra_tun", ip = "10.12.34.56", tun_ip;
    uint8_t subnet_mask = 24;
    tun_ip = ip + "/" + to_string(subnet_mask);
    bool tun_made = is_tun_made(tun_name);
    if(!tun_made){
        string exe_file = std::filesystem::canonical("/proc/self/exe");
        string tun_build_script = format(
            "sudo ip tuntap add dev {} mode tun user $(whoami) &&\\\n"
            "sudo ip link set {} up &&\\\n"
            "sudo ip addr add {} dev {}", 
            tun_name, tun_name, tun_ip, tun_name
        );
        fail(test_name, "tun %s doesn't exist!\nTry to launch it\n%s", -1, tun_name.c_str(), tun_build_script.c_str());
    }
    linux_tun tun(tun_name);
    tun.init();
    // заранее подготовленный кусок данных, который должен дойти как payload
    const char *test_payload = "korra_tun_read_test_payload";
    size_t payload_len = strlen(test_payload);

    string dst_ip = "10.12.34.57";   // любой адрес внутри подсети tun, отличный от tun_ip
    uint16_t dst_port = 9999;

    binary_semaphore sync(0);
    std::thread sender([&]() {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            fail(test_name, "failed to create sender socket: %s", -1, strerror(errno));
            return;
        }

        struct sockaddr_in dst_addr{};
        dst_addr.sin_family = AF_INET;
        dst_addr.sin_port = htons(dst_port);
        inet_pton(AF_INET, dst_ip.c_str(), &dst_addr.sin_addr);

        sync.acquire();
        for(int i =0; i < 2; i++){
            ssize_t sent = sendto(sock, test_payload, payload_len, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
            if (sent < 0) {
                fail(test_name, "sendto failed: %s", -1, strerror(errno));
            }
        }
        close(sock);
    });

    char *ip_packet = nullptr;
    
    while(ip_packet == nullptr){
        ip_packet = tun.read();
        sync.release();
        std::this_thread::sleep_for(chrono::milliseconds(10));
    }

    sender.join();

    ip_data data(ip_packet);
    auto header = (ipv4_header*)data.header();

    if (header->protocol != IPPROTO_UDP) {
        fail(test_name, "unexpected protocol in received packet: %d", -1, header->protocol);
    }

    int header_len = header->ihl * 4;
    char *udp_payload = ip_packet+header_len + 8; //+8 because size of udp header is 8 byte and it places after IP header
    bool equal = memcmp(udp_payload, test_payload, payload_len) == 0;

    if (!equal) {
        fail(test_name, "payload mismatch: expected '%s', got '%.*s'", -1, test_payload, udp_payload);
    }


    printPass(test_name);
    is_test_read = false;
}

void test_write(){
    is_test_write = true;
}

int main(){
    test_read();
    test_write();
    return 0;
}