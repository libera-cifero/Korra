cmake_minimum_required(VERSION 4.3.1)

project(stegvideo.test)

set(BIN_DIR ../../bin)
set(INCLUDE_DIR "../include" "include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

file(GLOB test_lib_src "lib/test.c")
file(GLOB frame_io_src "lib/frame_io.cpp")

file(GLOB color_test_src "src/color_test.cpp")
file(GLOB basic_block_test_src "src/encoder/provider/basic_block_test.cpp")
file(GLOB math_test_src "src/math_test.cpp")

file(GLOB frame_gen_tool_src "tool/frame_gen/frame_generator.cpp")

macro(target_setup target)
    target_link_directories(${target} PUBLIC ../bin/lib)
    target_include_directories(${target} PUBLIC ${INCLUDE_DIR})
endmacro()

macro(target_setup_tool target)
    target_setup(${target})
    set_target_properties(${target} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/tool
    )
endmacro()

#color_test
add_executable(color_test ${test_lib_src} ${color_test_src})
target_setup(color_test)
target_link_libraries(color_test PRIVATE color)

#basic_block_test
add_executable(basic_block_test ${test_lib_src} ${basic_block_test_src})
target_setup(basic_block_test)
target_link_libraries(basic_block_test PRIVATE basic_block)

#math_test
add_executable(math_test ${test_lib_src} ${math_test_src})
target_setup(math_test)
target_link_libraries(math_test PRIVATE math)

#frame_gen_tool
add_executable(frame_gen_tool ${frame_gen_tool_src} ${frame_io_src})
target_setup_tool(frame_gen_tool)
target_link_libraries(frame_gen_tool PRIVATE basic_block color rect rgb_index)