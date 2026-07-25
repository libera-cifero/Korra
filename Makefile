ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

#COMMON
#=====BEGIN=====
init_debug:
	python3 tool/cmake_init.py Debug
init_release:
	python3 tool/cmake_init.py Release
clean:
	cmake --build build --target clean

arch:
	dot -Tsvg docs/architecture.dot > docs/map.svg
arch_legend:
	dot -Tsvg docs/architecture_legend.dot > docs/arch_legend.svg
include_arch:
	dot -Tsvg docs/includes.dot > docs/include_map.svg
#======END======

#SRC
#=====BEGIN=====
base_utils:
	cmake --build build --target base_utils
color_codec:
	cmake --build build --target color_codec
mosaic:
	cmake --build build --target mosaic
len_reader: mosaic
	cmake --build build --target len_reader
timer: 
	cmake --build build --target timer

build_src: base_utils color_codec len_reader timer
#======END======

#TESTS
#=====BEGIN=====
color_test: color
	cmake --build build --target color_test && python3 tool/run.py color_test
mosaic_test: mosaic
	cmake --build build --target mosaic_test && python3 tool/run.py mosaic_test
math_test: math
	cmake --build build --target math_test && python3 tool/run.py math_test
timer_test: timer
	cmake --build build --target timer_test && python3 tool/run.py timer_test

test: color_test math_test timer_test mosaic_test
#======END======

#TEST TOOLS
#=====BEGIN=====
frame_gen_tool: mosaic color rect rgb_index color_codec
	cmake --build build --target frame_gen_tool
args_frame_gen_tool: color_codec
	cmake --build build --target args_frame_gen_tool
fps_color_tool:
	cmake --build build --target fps_color_tool
adm_tool: mosaic color_codec
	cmake --build build --target analyse_decode_metric_tool
gen_frame: frame_gen_tool
	python3 tool/run.py tool/frame_gen_tool $(ARGS)
gen_frames: frame_gen_tool args_frame_gen_tool
	python3 test/tool/gen_frames.py $(ARGS)
#======END======

%:
	@: