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

message(STATUS "DATA_BOXER_SRC: ${data_boxer_src}")

#lib
add_library(lib STATIC ${lib_src})
target_include_directories(lib PUBLIC ${INCLUDE_DIR})

#video_codec
add_library(video_codec STATIC ${video_codec_src})
target_include_directories(video_codec PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_codec PUBLIC lib ${CRYPTOPP_LIB})

#config
add_library(config STATIC ${config_src})
target_include_directories(config PUBLIC ${INCLUDE_DIR})
target_link_libraries(config PUBLIC lib video_codec)

#data_boxer
add_library(data_boxer STATIC ${data_boxer_src})
target_include_directories(data_boxer PUBLIC ${INCLUDE_DIR})
target_link_libraries(data_boxer PUBLIC lib video_codec config)

#tun
set(tun_src tun/tun.cpp tun/)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(tun_src ${tun_src}/linux_tun.cpp)
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()
add_library(tun STATIC ${tun_src})
target_include_directories(tun PUBLIC ${INCLUDE_DIR})