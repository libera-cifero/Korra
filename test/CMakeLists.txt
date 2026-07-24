cmake_minimum_required(VERSION 4.3.1)
project(korra.test)

set(INCLUDE_DIR ${INCLUDE_DIR} "include")

file(GLOB test_lib_src "lib/test.cpp")
file(GLOB frame_io_src "lib/frame_io.cpp")

file(GLOB color_test_src "src/color_test.cpp")
file(GLOB basic_block_test_src "src/encoder/provider/basic_block_test.cpp")
file(GLOB math_test_src "src/math_test.cpp")
file(GLOB len_reader_test_src "src/encoder/provider/len_reader_test.cpp")
file(GLOB timer_test_src "src/timer_test.cpp")
file(GLOB mosaic_test_src "src/encoder/provider/mosaic_provider_test.cpp")

file(GLOB frame_gen_tool_src "tool/gen_frame/frame_generator.cpp")
file(GLOB args_frame_gen_tool_src "tool/gen_frame/argument_generator.cpp")
file(GLOB fps_color_tool_src "tool/fps_color.cpp")
file(GLOB adm_tool_src "tool/analyse_decode/metric.cpp")

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

#math_test
add_executable(math_test ${test_lib_src} ${math_test_src})
target_setup(math_test)
target_link_libraries(math_test PUBLIC base_utils)

#basic_block_test
add_executable(basic_block_test ${test_lib_src} ${basic_block_test_src} ${frame_io_src})
target_setup(basic_block_test)
target_link_libraries(basic_block_test PRIVATE basic_block base_utils color_codec)

#len_reader_test
add_executable(len_reader_test ${test_lib_src} ${frame_io_src} ${len_reader_test_src})
target_setup(len_reader_test)
target_link_libraries(len_reader_test PUBLIC base_utils color_codec basic_block len_reader)

#timer_test
add_executable(timer_test ${test_lib_src} ${timer_test_src})
target_setup(timer_test)
target_link_libraries(timer_test PUBLIC timer Boost::filesystem)

#mosaic_test
add_executable(mosaic_test ${mosaic_test_src} ${frame_io_src} ${test_lib_src})
target_setup(mosaic_test)
target_link_libraries(mosaic_test PUBLIC mosaic color_codec)
#target_link_libraries(mosaic PUBLIC base_utils color_codec)

#======END======

#TEST TOOLS
#=====BEGIN=====
#frame_gen_tool
add_executable(frame_gen_tool ${test_lib_src} ${frame_gen_tool_src} ${frame_io_src})
target_setup_tool(frame_gen_tool)
target_link_libraries(frame_gen_tool PRIVATE basic_block base_utils color_codec)

#args_frame_gen_tool
add_executable(args_frame_gen_tool ${args_frame_gen_tool_src})
target_setup_tool(args_frame_gen_tool)
target_link_libraries(args_frame_gen_tool PRIVATE color_codec)

#fps_color_tool
add_executable(fps_color_tool ${fps_color_tool_src})
target_setup_tool(fps_color_tool)

#analyse_decode_metric_tool
add_executable(analyse_decode_metric_tool ${adm_tool_src} ${frame_io_src} ${test_lib_src})
target_setup_tool(analyse_decode_metric_tool)
target_link_libraries(analyse_decode_metric_tool PRIVATE basic_block color_codec)
#======END======