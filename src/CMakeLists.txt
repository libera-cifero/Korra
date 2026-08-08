cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

set(PROVIDER_DIR video_codec/frame_codec/provider)
set(CIPHER_DIR video_codec/frame_codec/cipher)
set(MOSAIC_DIR ${PROVIDER_DIR}/mosaic)

#lib/*
file(GLOB_RECURSE lib_src lib/*.cpp)

#video_codec/frame_codec/provider/mosaic/block_codec/*
file(GLOB block_codec_src ${MOSAIC_DIR}/block_codec/block_codec.cpp)
file(GLOB rgb_palette_codec_src ${MOSAIC_DIR}/block_codec/rgb_palette_codec.cpp)

#video_codec/frame_codec/provider/mosaic/*
file(GLOB mosaic_math_src ${MOSAIC_DIR}/mosaic_math.cpp)

#video_codec/frame_codec/provider/*
file(GLOB mosaic_provider_src ${PROVIDER_DIR}/mosaic_provider.cpp)

#video_codec/frame_codec/cipher/*
file(GLOB cipher_src ${CIPHER_DIR}/cipher.cpp)
file(GLOB none_cipher_src ${CIPHER_DIR}/none_cipher.cpp)

#video_codec/frame_codec/*
file(GLOB frame_codec_src video_codec/frame_codec/frame_codec.cpp)

#video_codec/*
file(GLOB sync_signals_src video_codec/sync_signals.cpp)
file(GLOB clock_src video_codec/clock_generator.cpp)
file(GLOB payload_storage_src video_codec/payload_storage.cpp)
file(GLOB video_codec_src video_codec/video_codec.cpp)

#video_codec/*
file(GLOB_RECURSE video_codec_src video_codec/*.cpp)

#config/*
file(GLOB_RECURSE config_src config/*.cpp)

message(STATUS "CONFIG SOURCES: ${config_src}")
message(STATUS "LIB SOURCES: ${lib_src}")
message(STATUS "VIDEO CODEC SOURCES: ${video_codec_src}")

#lib
add_library(lib STATIC ${lib_src})
target_include_directories(lib PUBLIC ${INCLUDE_DIR})

#video_codec
add_library(video_codec STATIC ${video_codec_src})
target_include_directories(video_codec PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_codec PUBLIC lib)

#config
add_library(config STATIC ${config_src})
target_include_directories(config PUBLIC ${INCLUDE_DIR})
target_link_libraries(config PUBLIC lib video_codec)