cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

set(PROVIDER_DIR video_codec/frame_codec/provider)
set(CIPHER_DIR video_codec/frame_codec/cipher)
set(MOSAIC_DIR ${PROVIDER_DIR}/mosaic)

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