cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

set(PROVIDER_DIR video_codec/frame_codec/provider)
set(CIPHER_DIR video_codec/frame_codec/cipher)
set(MOSAIC_DIR ${PROVIDER_DIR}/mosaic)
#lib/*
file(GLOB color_src lib/color.cpp)

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

#base_utils
add_library(base_utils OBJECT ${color_src})
target_include_directories(base_utils PUBLIC ${INCLUDE_DIR})

#block_codec
add_library(block_codec STATIC ${rgb_palette_codec_src} ${block_codec_src})
target_include_directories(block_codec PUBLIC ${INCLUDE_DIR})

#mosaic
add_library(mosaic STATIC ${mosaic_provider_src} ${mosaic_math_src})
target_include_directories(mosaic PUBLIC ${INCLUDE_DIR})
target_link_libraries(mosaic PUBLIC block_codec)

#clock_generator
add_library(clock_generator OBJECT ${clock_src} ${sync_signals_src})
target_include_directories(clock_generator PUBLIC ${INCLUDE_DIR})

#video_codec
add_library(video_codec STATIC ${sync_signals_src} ${clock_src} ${payload_storage_src} ${frame_codec_src} ${video_codec_src} ${cipher_src} ${none_cipher_src})
target_include_directories(video_codec PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_codec PUBLIC block_codec base_utils mosaic)