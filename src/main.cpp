#include "lib/log.hpp"
#include "video_socket.hpp"
#include "config/parser_factory/static_parser_factory.hpp"
#include "lib/CLI11.hpp"
#include <csignal>
#include <exception>
#include <filesystem>
#include <fstream>
#include <map>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
using namespace filesystem;
using namespace spdlog;

struct cli_args {
    level::level_enum log_level;
    path config_path;
};

auto socket_parser = static_parser_factory().build();
video_socket *socket;

video_socket *read_socket_from_file(path &file_path){
    string prefix = get_method_prefix("main.read_socket_from_file");
    video_socket *socket = nullptr;
    try{
        ifstream file(file_path);
        stringstream stream;
        stream << file.rdbuf();
        string config_str = stream.str();
        file.close();
        
        auto config = json::parse(config_str);
        socket = socket_parser->parse(config);
    }
    catch(exception &e){
        spdlog::error("{} invalid config!", prefix);
        spdlog::error("{} {}", prefix, e.what());
    }
    catch(...){
        spdlog::error("{} invalid config!", prefix);
    }
    return socket;
}

int parse_args(int argc, char **argv, cli_args &args){
    string prefix = get_method_prefix("main.parse_args");
    CLI::App app{"korra-proxy"};
    argv = app.ensure_utf8(argv);
    string log_level = "info", config_path;
    app.add_option("-l,--log-level", log_level);
    app.add_option("-c,--config", config_path)->required();
    CLI11_PARSE(app, argc, argv);
    map<string, level::level_enum> levels {
        { "trace", level::trace },
        { "debug", level::debug },
        { "info", level::info },
        { "warn", level::warn },
        { "err", level::err },
        { "critical", level::critical },
        { "off", level::off }
    };
    
    if(!levels.contains(log_level)) {
        string str = "";
        for(auto x = levels.begin(); x != levels.end(); x++){
            if(x != levels.begin()) str += ", ";
            str+=x->first;
        }
        spdlog::error("{} invalid log level {}! Available values: {}", prefix, log_level, str);
        return -1;
    }
    args.log_level = levels[log_level];
    try{
        args.config_path = current_path() / config_path;
    }
    catch(exception &e){
        spdlog::error("{} invalid path {}!", prefix, config_path);
        spdlog::error("{} {}!", prefix, e.what());
        return -1;
    }
    catch (...){
        spdlog::error("{} invalid path {}!", prefix, config_path);
        return -1;
    }

    return 0;
}

void free_context(){
    string prefix = get_method_prefix("main.free_context");
    spdlog::debug("{} context cleaning...", prefix);
    delete socket_parser;
    delete socket;
    spdlog::debug("{} context was cleaned successfully!", prefix);
}

void quit(int sig){
    string prefix = get_method_prefix("main.quit");
    map<int, string> signal_names={
        {SIGHUP, "SIGHUP"},
        {SIGINT, "SIGINT"},
        {SIGQUIT, "SIGQUIT"},
        {SIGABRT, "SIGABRT"},
        {SIGILL, "SIGILL" },
        {SIGSEGV, "SIGSEGV"},
        {SIGTERM, "SIGTERM"},
        {SIGSTOP, "SIGSTOP"},
        {SIGCHLD, "SIGCHLD"},
        {SIGALRM, "SIGALRM"},
        {SIGUSR1, "SIGUSR1"},
        {SIGUSR2, "SIGUSR2"}
    };
    if(sig == SIGSEGV) spdlog::critical("{} socket stopping by signal {}={}...", prefix, signal_names[sig], sig);
    else spdlog::info("{} socket stopping by signal {}={}...", prefix, signal_names[sig], sig);
    
    socket->stop();
    spdlog::info("{} socket stopped!", prefix);
    free_context();
    spdlog::info("{} program ended!", prefix);
    exit(0);
}

int main(int argc, char **argv){
    string prefix = get_method_prefix("main.main");
    cli_args args;
    int code = parse_args(argc, argv, args);
    if(code != 0) return code;
    spdlog::set_level(args.log_level);
    socket = read_socket_from_file(args.config_path);
    if(socket == nullptr) return -2;
    try{
        int signals[]{SIGINT, SIGTERM, SIGQUIT, SIGABRT, SIGSEGV};
        for(int i = 0; i < sizeof(signals) / sizeof(int); i++) signal(signals[i], quit);
        socket->run();
    }
    catch(exception &e){
        spdlog::critical("{} {}", prefix, e.what());
        free_context();
        return -3;
    }
    catch(...){
        spdlog::critical("{} something went wrong!", prefix);
        free_context();
        return -3;
    }
    return 0;
}