#include "io.hpp"
#include "lib/CLI11.hpp"
#include <filesystem>

path to_native_path(std::string file_name){
    auto strs = CLI::detail::split(file_name, '/');
    path p;
    for(std::string str : strs) p /= str;
    return p;
}