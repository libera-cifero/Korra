#pragma once
#include <functional>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
#define BYTE_STR_DISPLAY_COUNT 20
using namespace std;
enum log_mode : unsigned char {
    NORMAL = 0,
    TRACE_ONLY = 1
};
string get_method_prefix(string method_name);
bool is_debug_level();
string get_byte_str(void *bytes);
template <typename... Args>
void log_with_mode(spdlog::level::level_enum log_level, log_mode mode, spdlog::format_string_t<Args...> fmt, Args &&...args){
    if(mode == log_mode::NORMAL){
        switch (log_level) {
            case spdlog::level::trace:
                spdlog::trace(fmt, std::forward<Args>(args)...);
                break;
            case spdlog::level::debug:
                spdlog::debug(fmt, std::forward<Args>(args)...);
                break;
            case spdlog::level::info:
                spdlog::info(fmt, std::forward<Args>(args)...);
                break;
            case spdlog::level::warn:
                spdlog::warn(fmt, std::forward<Args>(args)...);
                break;
            case spdlog::level::err:
                spdlog::error(fmt, std::forward<Args>(args)...);
                break;
            case spdlog::level::critical:
                spdlog::critical(fmt, std::forward<Args>(args)...);
                break;
            default:
                break;
        }
    }
    else spdlog::trace(fmt, std::forward<Args>(args)...);
}