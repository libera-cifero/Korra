cmake_minimum_required(VERSION 4.3.1)
project(korra.test)

set(INCLUDE_DIR ${INCLUDE_DIR} "include")

file(GLOB test_lib_src "lib/test.cpp")
file(GLOB frame_io_src "lib/frame_io.cpp")
file(GLOB block_codec_json_src "lib/block_codec_json.cpp")
file(GLOB time_src "lib/time.cpp")

file(GLOB color_test_src "src/color_test.cpp")
file(GLOB mosaic_test_src "src/video_codec/frame_codec/provider/mosaic_provider_test.cpp")
file(GLOB clock_gen_test_src "src/video_codec/clock_generator_test.cpp")
file(GLOB video_codec_test_src "src/video_codec/video_codec_test.cpp")
file(GLOB video_config_parser_test_src "src/parser/video_config_parser_test.cpp")
file(GLOB frame_codec_parser_test_src "src/parser/frame_codec_parser_test.cpp")

file(GLOB frame_gen_tool_src "tool/gen_frame/frame_generator.cpp")
file(GLOB args_frame_gen_tool_src "tool/gen_frame/argument_generator.cpp")
file(GLOB fps_color_tool_src "tool/fps_color.cpp")

macro(target_setup target)
    target_link_directories(${target} PUBLIC ${LIB_DIR})
    target_include_directories(${target} PUBLIC ${INCLUDE_DIR})
endmacro()

macro(target_setup_tool target)
    target_setup(${target})
    set_target_properties(${target} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/tool
    )
endmacro()

#TESTS
#=====BEGIN=====
#color_test
add_executable(color_test ${test_lib_src} ${color_test_src})
target_setup(color_test)
target_link_libraries(color_test PUBLIC base_utils)

#mosaic_test
add_executable(mosaic_test ${mosaic_test_src} ${time_src} ${frame_io_src} ${test_lib_src} ${block_codec_json_src})
target_setup(mosaic_test)
target_link_libraries(mosaic_test PUBLIC block_codec base_utils mosaic)
#target_link_libraries(mosaic PUBLIC base_utils color_codec)

#clock_generator_test
add_executable(clock_generator_test ${clock_gen_test_src})
target_setup(clock_generator_test)
target_link_libraries(clock_generator_test PUBLIC clock_generator)

#video_codec_test
add_executable(video_codec_test ${video_codec_test_src} ${test_lib_src} ${block_codec_json_src} ${frame_io_src})
target_setup(video_codec_test)
target_link_libraries(video_codec_test PUBLIC video_codec)

#video_config_parser_test
add_executable(video_config_parser_test ${test_lib_src} ${video_config_parser_test_src})
target_setup(video_config_parser_test)
target_link_libraries(video_config_parser_test PUBLIC config)

#frame_codec_parser_test
add_executable(frame_codec_parser_test ${test_lib_src} ${frame_codec_parser_test_src})
target_setup(frame_codec_parser_test)
target_link_libraries(frame_codec_parser_test PUBLIC config video_codec)
#======END======

#TEST TOOLS
#=====BEGIN=====
#frame_gen_tool
add_executable(frame_gen_tool ${test_lib_src} ${frame_gen_tool_src} ${frame_io_src} ${block_codec_json_src})
target_setup_tool(frame_gen_tool)
target_link_libraries(frame_gen_tool PRIVATE mosaic block_codec base_utils)

#args_frame_gen_tool
add_executable(args_frame_gen_tool ${args_frame_gen_tool_src})
target_setup_tool(args_frame_gen_tool)
target_link_libraries(args_frame_gen_tool PRIVATE block_codec)

#fps_color_tool
add_executable(fps_color_tool ${fps_color_tool_src})
target_setup_tool(fps_color_tool)
#======END======