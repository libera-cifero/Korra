cmake_minimum_required(VERSION 4.3.1)
project(korra.src)

set(mosaic_DIR "video/encoder/provider/mosaic")

file(GLOB color_src "color.cpp")
file(GLOB math_src "math.cpp")

file(GLOB mosaic_config_src "${mosaic_DIR}/mosaic_settings.cpp")
file(GLOB color_codec_src "${mosaic_DIR}/color_codec/color_codec.cpp")
file(GLOB hsv_codec_src "${mosaic_DIR}/color_codec/hsv_codec.cpp")
file(GLOB palette_codec_src "${mosaic_DIR}/color_codec/palette_codec.cpp")
file(GLOB rgb_palette_codec_src "${mosaic_DIR}/color_codec/rgb_palette_codec.cpp")
file(GLOB ycbcr_palette_codec_src "${mosaic_DIR}/color_codec/ycbcr_palette_codec.cpp")
file(GLOB codec_json_src "${mosaic_DIR}/color_codec/codec_json.cpp")
file(GLOB mosaic_math_src "${mosaic_DIR}/mosaic_math.cpp")
file(GLOB mosaic_provider_src "video/encoder/provider/mosaic_provider.cpp")

file(GLOB len_reader_src "video/encoder/provider/len_reader/raw_ip_length_reader.cpp")
file(GLOB bbpe_src "estimator/mosaic_plain_estimator.cpp")


#base_utils
add_library(base_utils OBJECT ${color_src} ${math_src})
target_include_directories(base_utils PUBLIC ${INCLUDE_DIR})

#color_codec
add_library(color_codec OBJECT ${color_codec_src} ${hsv_codec_src} ${palette_codec_src} ${rgb_palette_codec_src} ${ycbcr_palette_codec_src} ${color_src} ${codec_json_src} ${mosaic_math_src})
target_include_directories(color_codec PUBLIC ${INCLUDE_DIR})

#timer
add_library(timer OBJECT "data_boxer/timer.cpp")
target_include_directories(timer PUBLIC ${INCLUDE_DIR})

#mosaic
add_library(mosaic STATIC ${mosaic_provider_src} ${mosaic_math_src})
target_include_directories(mosaic PUBLIC ${INCLUDE_DIR})