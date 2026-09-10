cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

#lib/*
file(GLOB_RECURSE lib_src lib/*.cpp)

#video_codec/*
file(GLOB_RECURSE video_codec_src video_codec/*.cpp)

#config/*
file(GLOB_RECURSE config_src config/*.cpp)

#data_boxer/*
file(GLOB_RECURSE data_boxer_src data_boxer/*.cpp)

#pipe/*
file(GLOB_RECURSE pipe_src pipe/*.cpp)

#event/*
file(GLOB_RECURSE event_src event/*.cpp)

#video_socket_src
file(GLOB_RECURSE video_socket_src video_socket.cpp video_stream_in.cpp video_stream_out.cpp)

#lib
add_library(lib STATIC ${lib_src})
target_include_directories(lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(lib PUBLIC spdlog)

#video_codec
add_library(video_codec STATIC ${video_codec_src})
target_include_directories(video_codec PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_codec PUBLIC lib ${CRYPTOPP_LIB} spdlog)

#data_boxer
add_library(data_boxer STATIC ${data_boxer_src})
target_include_directories(data_boxer PUBLIC ${INCLUDE_DIR})
target_link_libraries(data_boxer PUBLIC spdlog lib video_codec)

#tun
set(tun_src tun/tun.cpp tun/tun_builder.cpp tun/)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(tun_src ${tun_src}/linux_tun.cpp)
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()
add_library(tun STATIC ${tun_src})
target_include_directories(tun PUBLIC ${INCLUDE_DIR})
target_link_libraries(tun PUBLIC spdlog)

#video_socket
add_library(video_socket STATIC ${video_socket_src} ${pipe_src} ${event_src})
target_include_directories(video_socket PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_socket PUBLIC spdlog lib video_codec data_boxer tun)

#config
add_library(config SHARED ${config_src})
target_include_directories(config PUBLIC ${INCLUDE_DIR})
target_link_libraries(config PUBLIC spdlog lib video_codec data_boxer tun video_socket)