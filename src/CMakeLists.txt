cmake_minimum_required(VERSION 4.3.1)
project(stegvideo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BIN_DIR ../../bin)
set(PROVIDER_DIR encoder/provider)
set(BASIC_BLOCK_DIR "encoder/provider/basic_block")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB color_src "color.cpp")
file(GLOB math_src "math.c")
file(GLOB basic_block_config_src "encoder/provider/basic_block_config.cpp")
file(GLOB bbc_src "${BASIC_BLOCK_DIR}/basic_block_container.cpp")
file(GLOB bbp_src "${BASIC_BLOCK_DIR}/basic_block_pointer.cpp")
file(GLOB bbpp_src "${BASIC_BLOCK_DIR}/basic_block_pointer_proxy.cpp")
file(GLOB rect_src "${BASIC_BLOCK_DIR}/rect.c")
file(GLOB rgb_index_src "${BASIC_BLOCK_DIR}/rgb_index.c")
file(GLOB color_codec_src "color_codec/color_codec.cpp")
file(GLOB hsv_codec_src "color_codec/hsv_codec.cpp")
file(GLOB palette_codec_src "color_codec/palette_codec.cpp")
file(GLOB rgb_palette_codec_src "color_codec/rgb_palette_codec.cpp")
file(GLOB ycbcr_palette_codec_src "color_codec/ycbcr_palette_codec.cpp")
file(GLOB codec_json_src "color_codec/codec_json.cpp")
file(GLOB len_reader_src "encoder/provider/len_reader/raw_ip_length_reader.cpp")

#base_utils
add_library(base_utils STATIC ${color_src} ${math_src})
target_include_directories(base_utils PUBLIC ../include)

#color_codec
add_library(color_codec STATIC ${color_codec_src} ${hsv_codec_src} ${palette_codec_src} ${rgb_palette_codec_src} ${ycbcr_palette_codec_src} ${color_src} ${codec_json_src})
target_include_directories(color_codec PUBLIC ../include)

#basic_block
add_library(basic_block SHARED ${bbc_src} ${bbp_src} ${bbpp_src} ${basic_block_config_src} ${rect_src} ${rgb_index_src})
target_link_libraries(basic_block PUBLIC base_utils color_codec)
target_include_directories(basic_block PUBLIC ../include)

#len_reader
add_library(len_reader SHARED ${len_reader_src})
target_link_libraries(len_reader PUBLIC basic_block)
target_include_directories(len_reader PUBLIC ../include)