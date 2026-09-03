#pragma once
#include <string>
#define BYTE_STR_DISPLAY_COUNT 20
using namespace std;

string get_method_prefix(string method_name);
bool is_debug_level();
string get_byte_str(void *bytes);