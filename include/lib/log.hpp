#pragma once
#include "data/ip_data.hpp"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
#define BYTE_STR_DISPLAY_COUNT 20
using namespace std;
string get_method_prefix(string method_name);
bool is_debug_level();
string get_byte_str(void *bytes);
std::string format_ip_header(ip_header* header);