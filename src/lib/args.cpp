#include "lib/args.hpp"
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

char** split_args(const string& cmd, int &args_count) {
    std::istringstream iss(cmd);
    vector<string> args;
    std::string arg;

    while (iss >> arg) args.push_back(arg);
    args_count = args.size();
    char **char_args = new char*[args_count + 1];
    for(int i = 0; i < args_count; i++){
        int size = args[i].size() + 1;
        char *char_arg = new char[size];
        memcpy(char_arg, args[i].c_str(), size);
        char_args[i] = char_arg;
    }
    char_args[args_count] = NULL;
    
    return char_args;
}