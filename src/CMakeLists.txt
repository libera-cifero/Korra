cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

set(mosaic_DIR "video_codec/frame_codec/provider/mosaic")

file(GLOB color_src "lib/color.cpp")

file(GLOB mosaic_config_src "${mosaic_DIR}/mosaic_settings.cpp")
file(GLOB color_codec_src "${mosaic_DIR}/color_codec/color_codec.cpp")
file(GLOB hsv_codec_src "${mosaic_DIR}/color_codec/hsv_codec.cpp")
file(GLOB palette_codec_src "${mosaic_DIR}/color_codec/palette_codec.cpp")
file(GLOB rgb_palette_codec_src "${mosaic_DIR}/color_codec/rgb_palette_codec.cpp")
file(GLOB ycbcr_palette_codec_src "${mosaic_DIR}/color_codec/ycbcr_palette_codec.cpp")
file(GLOB codec_json_src "${mosaic_DIR}/color_codec/codec_json.cpp")
file(GLOB mosaic_math_src "${mosaic_DIR}/mosaic_math.cpp")
file(GLOB mosaic_provider_src "video_codec/frame_codec/provider/mosaic_provider.cpp")
file(GLOB cipher_src "video_codec/frame_codec/cipher/cipher.cpp")
file(GLOB none_cipher_src "video_codec/frame_codec/cipher/none_cipher.cpp")
file(GLOB frame_codec_src "video_codec/frame_codec/frame_codec.cpp")
file(GLOB clock_src "video_codec/clock_generator.cpp")
file(GLOB sync_signals_src "video_codec/sync_signals.cpp")
file(GLOB payload_storage_src "video_codec/payload_storage.cpp")
file(GLOB video_codec_src "video_codec/video_codec.cpp")

#base_utils
add_library(base_utils OBJECT ${color_src})
target_include_directories(base_utils PUBLIC ${INCLUDE_DIR})

#color_codec
add_library(color_codec OBJECT ${color_codec_src} ${hsv_codec_src} ${palette_codec_src} ${rgb_palette_codec_src} ${ycbcr_palette_codec_src} ${color_src} ${codec_json_src} ${mosaic_math_src})
target_include_directories(color_codec PUBLIC ${INCLUDE_DIR})

#mosaic
add_library(mosaic STATIC ${mosaic_provider_src} ${mosaic_math_src})
target_include_directories(mosaic PUBLIC ${INCLUDE_DIR})

#video_codec
add_library(video_codec STATIC ${sync_signals_src} ${clock_src} ${payload_storage_src} ${frame_codec_src} ${video_codec_src} ${cipher_src} ${none_cipher_src})
target_include_directories(video_codec PUBLIC ${INCLUDE_DIR})
target_link_libraries(video_codec PUBLIC color_codec base_utils mosaic)