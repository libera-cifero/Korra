cmake_minimum_required(VERSION 4.3.1)
project(korra.test)

set(INCLUDE_DIR ${INCLUDE_DIR} "include")

file(GLOB test_lib_src "lib/test.cpp")
file(GLOB frame_io_src "lib/frame_io.cpp")

file(GLOB color_test_src "src/color_test.cpp")
file(GLOB mosaic_test_src "src/video_encoder/provider/mosaic_provider_test.cpp")
file(GLOB clock_gen_test_src "src/video_encoder/clock_generator_test.cpp")

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
add_executable(mosaic_test ${mosaic_test_src} ${frame_io_src} ${test_lib_src})
target_setup(mosaic_test)
target_link_libraries(mosaic_test PUBLIC mosaic color_codec)
#target_link_libraries(mosaic PUBLIC base_utils color_codec)

#clock_generator_test
add_executable(clock_generator_test ${clock_gen_test_src})
target_setup(clock_generator_test)
target_link_libraries(clock_generator_test PUBLIC oscillator)

#======END======

#TEST TOOLS
#=====BEGIN=====
#frame_gen_tool
add_executable(frame_gen_tool ${test_lib_src} ${frame_gen_tool_src} ${frame_io_src})
target_setup_tool(frame_gen_tool)
target_link_libraries(frame_gen_tool PRIVATE mosaic color_codec)

#args_frame_gen_tool
add_executable(args_frame_gen_tool ${args_frame_gen_tool_src})
target_setup_tool(args_frame_gen_tool)
target_link_libraries(args_frame_gen_tool PRIVATE color_codec)

#fps_color_tool
add_executable(fps_color_tool ${fps_color_tool_src})
target_setup_tool(fps_color_tool)
#======END======