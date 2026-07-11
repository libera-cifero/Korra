ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

#COMMON
#=====BEGIN=====
init_release: init_src_release init_test_release
init_debug: init_src_debug init_test_debug

arch:
	dot -Tsvg doc/architecture.dot > doc/map.svg
include_arch:
	dot -Tsvg doc/includes.dot > doc/include_map.svg
#======END======

#SRC
#=====BEGIN=====
init_src_debug:
	python3 tool/cmake_init.py src Debug
init_src_release:
	python3 tool/cmake_init.py src Release
build_src:
	cmake --build build/src

base_utils:
	cmake --build build/src --target base_utils
color_codec:
	cmake --build build/src --target color_codec
basic_block: base_utils color_codec
	cmake --build build/src --target basic_block
len_reader: basic_block
	cmake --build build/src --target len_reader
#======END======

#TESTS
#=====BEGIN=====
init_test_debug:
	python3 tool/cmake_init.py test Debug
init_test_release:
	python3 tool/cmake_init.py test Release
build_test: build_src
	cmake --build build/test

test: build_test
	python3 tool/test.py
color_test: color
	cmake --build build/test --target color_test && python3 tool/run.py color_test
basic_block_test: basic_block
	cmake --build build/test --target basic_block_test && python3 tool/run.py basic_block_test
math_test: math
	cmake --build build/test --target math_test && python3 tool/run.py math_test
len_reader_test: len_reader
	cmake --build build/test --target len_reader_test && python3 tool/run.py len_reader_test
#======END======

#TEST TOOLS
#=====BEGIN=====
frame_gen_tool: basic_block color rect rgb_index color_codec
	cmake --build build/test --target frame_gen_tool
args_frame_gen_tool: color_codec
	cmake --build build/test --target args_frame_gen_tool
fps_color_tool:
	cmake --build build/test --target fps_color_tool
adm_tool: basic_block color_codec
	cmake --build build/test --target analyse_decode_metric_tool
gen_frame: frame_gen_tool
	python3 tool/run.py tool/frame_gen_tool $(ARGS)
gen_frames: frame_gen_tool args_frame_gen_tool
	python3 test/tool/gen_frames.py $(ARGS)
#======END======

%:
	@: