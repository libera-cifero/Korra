#include <ftxui/component/app.hpp>
#include <spdlog/spdlog.h>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <string>
#include <ftxui/ftxui.hpp>

using namespace std;
using namespace ftxui;

Component build_ui(){
    
}

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