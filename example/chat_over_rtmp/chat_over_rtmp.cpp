#include <spdlog/spdlog.h>
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char **argv){
    spdlog::set_level(spdlog::level::off);
    string username, self_ip, dest_ep;
    cout << "Your IP >> ";
    cin >> self_ip;
    cout << "Your name >> ";
    cin >> username;
    cout << "Dest endpoint >> ";

    return 0;
}