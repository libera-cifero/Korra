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
clock_gen_test:
	cmake --build build --target clock_generator_test && python3 tool/run.py clock_generator_test
video_encoder_test:
	cmake --build build --target video_encoder_test
test: color_test math_test timer_test mosaic_test
#======END======

#TEST TOOLS
#=====BEGIN=====
frame_gen_tool:
	cmake --build build --target frame_gen_tool
args_frame_gen_tool:
	cmake --build build --target args_frame_gen_tool
fps_color_tool:
	cmake --build build --target fps_color_tool

#args: frame_width, frame_height, color_codec_path, block_size, name of expected data file, name of frame file
#color_codec_path must be path relative the test/context/data/color_codec
#name of expected data file must be path relative the test/context/expected/frame
#name of frame file must be path relative the test/context/data/frame
gen_frame: frame_gen_tool
	python3 tool/run.py tool/frame_gen_tool $(ARGS)

#ARGS: count, palette_path, width, height, block_size, folder. 
#Width, height and folder aren't required here. If they aren't defined, then they will be generated 
#If you wanna generate random width and/or height, just set 0
gen_frames: frame_gen_tool args_frame_gen_tool
	python3 test/tool/gen_frames.py $(ARGS)
#======END======

%:
	@: