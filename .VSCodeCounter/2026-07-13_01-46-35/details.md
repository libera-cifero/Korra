# Details

Date : 2026-07-13 01:46:35

Directory /home/admin/programming/C++/Korra/stegvideo

Total : 118 files,  61086 codes, 4772 comments, 3736 blanks, all 69594 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [Makefile](/Makefile) | Makefile | 65 | 0 | 8 | 73 |
| [docs/README.md](/docs/README.md) | Markdown | 29 | 0 | 2 | 31 |
| [docs/arch_legend.svg](/docs/arch_legend.svg) | XML | 202 | 47 | 1 | 250 |
| [docs/architecture.dot](/docs/architecture.dot) | Graphviz (DOT) | 183 | 0 | 45 | 228 |
| [docs/architecture_legend.dot](/docs/architecture_legend.dot) | Graphviz (DOT) | 65 | 0 | 19 | 84 |
| [docs/include_map.svg](/docs/include_map.svg) | XML | 75 | 16 | 1 | 92 |
| [docs/includes.dot](/docs/includes.dot) | Graphviz (DOT) | 16 | 0 | 1 | 17 |
| [docs/map.svg](/docs/map.svg) | XML | 510 | 97 | 1 | 608 |
| [include/color.hpp](/include/color.hpp) | C++ | 14 | 0 | 5 | 19 |
| [include/color_codec/codec_json.hpp](/include/color_codec/codec_json.hpp) | C++ | 5 | 0 | 1 | 6 |
| [include/color_codec/color_codec.hpp](/include/color_codec/color_codec.hpp) | C++ | 13 | 0 | 0 | 13 |
| [include/color_codec/hsv_codec.hpp](/include/color_codec/hsv_codec.hpp) | C++ | 14 | 0 | 2 | 16 |
| [include/color_codec/palette_codec.hpp](/include/color_codec/palette_codec.hpp) | C++ | 15 | 0 | 1 | 16 |
| [include/color_codec/rgb_palette_codec.hpp](/include/color_codec/rgb_palette_codec.hpp) | C++ | 7 | 0 | 1 | 8 |
| [include/color_codec/ycbcr_palette_codec.hpp](/include/color_codec/ycbcr_palette_codec.hpp) | C++ | 7 | 0 | 1 | 8 |
| [include/encoder/compressor/compressor.hpp](/include/encoder/compressor/compressor.hpp) | C++ | 7 | 0 | 0 | 7 |
| [include/encoder/compressor/none_compressor.hpp](/include/encoder/compressor/none_compressor.hpp) | C++ | 6 | 0 | 1 | 7 |
| [include/encoder/corrector/corrector.hpp](/include/encoder/corrector/corrector.hpp) | C++ | 7 | 0 | 0 | 7 |
| [include/encoder/corrector/none_recoverer.hpp](/include/encoder/corrector/none_recoverer.hpp) | C++ | 6 | 0 | 1 | 7 |
| [include/encoder/obfuscator/none_obfuscator.hpp](/include/encoder/obfuscator/none_obfuscator.hpp) | C++ | 6 | 0 | 1 | 7 |
| [include/encoder/obfuscator/obfuscator.hpp](/include/encoder/obfuscator/obfuscator.hpp) | C++ | 7 | 0 | 0 | 7 |
| [include/encoder/provider/basic_block/basic_block_container.hpp](/include/encoder/provider/basic_block/basic_block_container.hpp) | C++ | 32 | 0 | 7 | 39 |
| [include/encoder/provider/basic_block/basic_block_pointer.hpp](/include/encoder/provider/basic_block/basic_block_pointer.hpp) | C++ | 46 | 2 | 12 | 60 |
| [include/encoder/provider/basic_block/basic_block_pointer_proxy.hpp](/include/encoder/provider/basic_block/basic_block_pointer_proxy.hpp) | C++ | 19 | 2 | 4 | 25 |
| [include/encoder/provider/basic_block/bit_area.h](/include/encoder/provider/basic_block/bit_area.h) | C++ | 5 | 0 | 0 | 5 |
| [include/encoder/provider/basic_block/rect.h](/include/encoder/provider/basic_block/rect.h) | C++ | 14 | 0 | 4 | 18 |
| [include/encoder/provider/basic_block/rgb_index.h](/include/encoder/provider/basic_block/rgb_index.h) | C++ | 13 | 0 | 3 | 16 |
| [include/encoder/provider/basic_block_config.hpp](/include/encoder/provider/basic_block_config.hpp) | C++ | 16 | 1 | 2 | 19 |
| [include/encoder/provider/basic_provider.hpp](/include/encoder/provider/basic_provider.hpp) | C++ | 21 | 0 | 2 | 23 |
| [include/encoder/provider/len_reader/data_length_reader.hpp](/include/encoder/provider/len_reader/data_length_reader.hpp) | C++ | 7 | 0 | 0 | 7 |
| [include/encoder/provider/len_reader/raw_ip_length_reader.hpp](/include/encoder/provider/len_reader/raw_ip_length_reader.hpp) | C++ | 17 | 0 | 2 | 19 |
| [include/encoder/provider/provider.hpp](/include/encoder/provider/provider.hpp) | C++ | 7 | 0 | 0 | 7 |
| [include/encoder/video_encoder.hpp](/include/encoder/video_encoder.hpp) | C++ | 20 | 0 | 2 | 22 |
| [include/json.hpp](/include/json.hpp) | C++ | 18,408 | 4,566 | 3,138 | 26,112 |
| [include/math.h](/include/math.h) | C++ | 9 | 0 | 3 | 12 |
| [include/status_error.hpp](/include/status_error.hpp) | C++ | 12 | 0 | 0 | 12 |
| [include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_config.hpp](/include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_config.hpp) | C++ | 5 | 0 | 1 | 6 |
| [include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp](/include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.hpp) | C++ | 13 | 0 | 1 | 14 |
| [include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp](/include/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.hpp) | C++ | 13 | 0 | 2 | 15 |
| [include/video_pipe/video_pipe_config.hpp](/include/video_pipe/video_pipe_config.hpp) | C++ | 5 | 0 | 0 | 5 |
| [include/video_pipe/video_pipe_in.hpp](/include/video_pipe/video_pipe_in.hpp) | C++ | 7 | 0 | 1 | 8 |
| [include/video_pipe/video_pipe_out.hpp](/include/video_pipe/video_pipe_out.hpp) | C++ | 7 | 0 | 1 | 8 |
| [include/video_socket.hpp](/include/video_socket.hpp) | C++ | 5 | 1 | 0 | 6 |
| [src/color.cpp](/src/color.cpp) | C++ | 127 | 0 | 14 | 141 |
| [src/color_codec/codec_json.cpp](/src/color_codec/codec_json.cpp) | C++ | 116 | 0 | 15 | 131 |
| [src/color_codec/color_codec.cpp](/src/color_codec/color_codec.cpp) | C++ | 23 | 0 | 6 | 29 |
| [src/color_codec/hsv_codec.cpp](/src/color_codec/hsv_codec.cpp) | C++ | 93 | 0 | 10 | 103 |
| [src/color_codec/palette_codec.cpp](/src/color_codec/palette_codec.cpp) | C++ | 36 | 0 | 6 | 42 |
| [src/color_codec/rgb_palette_codec.cpp](/src/color_codec/rgb_palette_codec.cpp) | C++ | 9 | 0 | 3 | 12 |
| [src/color_codec/ycbcr_palette_codec.cpp](/src/color_codec/ycbcr_palette_codec.cpp) | C++ | 7 | 0 | 1 | 8 |
| [src/encoder/provider/basic_block/basic_block_container.cpp](/src/encoder/provider/basic_block/basic_block_container.cpp) | C++ | 116 | 1 | 21 | 138 |
| [src/encoder/provider/basic_block/basic_block_pointer.cpp](/src/encoder/provider/basic_block/basic_block_pointer.cpp) | C++ | 109 | 0 | 23 | 132 |
| [src/encoder/provider/basic_block/basic_block_pointer_proxy.cpp](/src/encoder/provider/basic_block/basic_block_pointer_proxy.cpp) | C++ | 45 | 2 | 8 | 55 |
| [src/encoder/provider/basic_block/rect.c](/src/encoder/provider/basic_block/rect.c) | C | 13 | 0 | 3 | 16 |
| [src/encoder/provider/basic_block/rgb_index.c](/src/encoder/provider/basic_block/rgb_index.c) | C | 7 | 0 | 2 | 9 |
| [src/encoder/provider/basic_block_config.cpp](/src/encoder/provider/basic_block_config.cpp) | C++ | 5 | 0 | 1 | 6 |
| [src/encoder/provider/basic_provider.cpp](/src/encoder/provider/basic_provider.cpp) | C++ | 11 | 0 | 6 | 17 |
| [src/encoder/provider/len_reader/raw_ip_length_reader.cpp](/src/encoder/provider/len_reader/raw_ip_length_reader.cpp) | C++ | 38 | 0 | 6 | 44 |
| [src/encoder/video_encoder.cpp](/src/encoder/video_encoder.cpp) | C++ | 12 | 0 | 3 | 15 |
| [src/main.c](/src/main.c) | C | 51 | 0 | 14 | 65 |
| [src/math.c](/src/math.c) | C | 12 | 0 | 2 | 14 |
| [src/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.cpp](/src/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_in.cpp) | C++ | 29 | 0 | 6 | 35 |
| [src/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.cpp](/src/video_pipe/ffmpeg_rtmp/ffmpeg_rtmp_pipe_out.cpp) | C++ | 31 | 0 | 5 | 36 |
| [test/context/data/color_codec/rgb_palette0.json](/test/context/data/color_codec/rgb_palette0.json) | JSON | 24 | 0 | 0 | 24 |
| [test/context/data/color_codec/rgb_palette1.json](/test/context/data/color_codec/rgb_palette1.json) | JSON | 40 | 0 | 0 | 40 |
| [test/context/data/color_codec/rgb_palette2.json](/test/context/data/color_codec/rgb_palette2.json) | JSON | 72 | 0 | 0 | 72 |
| [test/context/data/config/template.json](/test/context/data/config/template.json) | JSON | 61 | 0 | 5 | 66 |
| [test/context/expected/frame/frame_37.json](/test/context/expected/frame/frame_37.json) | JSON | 8,088 | 0 | 0 | 8,088 |
| [test/context/expected/frame/frame_38.json](/test/context/expected/frame/frame_38.json) | JSON | 136 | 0 | 0 | 136 |
| [test/context/expected/frame/frame_39.json](/test/context/expected/frame/frame_39.json) | JSON | 6,168 | 0 | 0 | 6,168 |
| [test/context/expected/frame/frame_40.json](/test/context/expected/frame/frame_40.json) | JSON | 72 | 0 | 0 | 72 |
| [test/context/expected/frame/frame_41.json](/test/context/expected/frame/frame_41.json) | JSON | 48 | 0 | 0 | 48 |
| [test/context/expected/frame/frame_42.json](/test/context/expected/frame/frame_42.json) | JSON | 136 | 0 | 0 | 136 |
| [test/context/expected/frame/frame_43.json](/test/context/expected/frame/frame_43.json) | JSON | 1,968 | 0 | 0 | 1,968 |
| [test/context/expected/frame/frame_44.json](/test/context/expected/frame/frame_44.json) | JSON | 816 | 0 | 0 | 816 |
| [test/context/expected/frame/frame_45.json](/test/context/expected/frame/frame_45.json) | JSON | 40 | 0 | 0 | 40 |
| [test/context/expected/frame/frame_46.json](/test/context/expected/frame/frame_46.json) | JSON | 2,048 | 0 | 0 | 2,048 |
| [test/context/expected/frame/frame_47.json](/test/context/expected/frame/frame_47.json) | JSON | 64 | 0 | 0 | 64 |
| [test/context/expected/frame/frame_48.json](/test/context/expected/frame/frame_48.json) | JSON | 40 | 0 | 0 | 40 |
| [test/context/expected/frame/frame_49.json](/test/context/expected/frame/frame_49.json) | JSON | 56 | 0 | 0 | 56 |
| [test/context/expected/frame/frame_50.json](/test/context/expected/frame/frame_50.json) | JSON | 4,792 | 0 | 0 | 4,792 |
| [test/context/expected/frame/frame_51.json](/test/context/expected/frame/frame_51.json) | JSON | 2,800 | 0 | 0 | 2,800 |
| [test/context/expected/frame/frame_52.json](/test/context/expected/frame/frame_52.json) | JSON | 48 | 0 | 0 | 48 |
| [test/context/expected/frame/frame_53.json](/test/context/expected/frame/frame_53.json) | JSON | 168 | 0 | 0 | 168 |
| [test/context/expected/frame/frame_54.json](/test/context/expected/frame/frame_54.json) | JSON | 168 | 0 | 0 | 168 |
| [test/context/expected/frame/frame_55.json](/test/context/expected/frame/frame_55.json) | JSON | 448 | 0 | 0 | 448 |
| [test/context/expected/frame/frame_56.json](/test/context/expected/frame/frame_56.json) | JSON | 9,048 | 0 | 0 | 9,048 |
| [test/context/expected/frame/frame_57.json](/test/context/expected/frame/frame_57.json) | JSON | 208 | 0 | 0 | 208 |
| [test/context/expected/frame/frame_58.json](/test/context/expected/frame/frame_58.json) | JSON | 128 | 0 | 0 | 128 |
| [test/context/expected/frame/frame_59.json](/test/context/expected/frame/frame_59.json) | JSON | 560 | 0 | 0 | 560 |
| [test/context/expected/frame/frame_60.json](/test/context/expected/frame/frame_60.json) | JSON | 200 | 0 | 0 | 200 |
| [test/context/expected/frame/frame_61.json](/test/context/expected/frame/frame_61.json) | JSON | 200 | 0 | 0 | 200 |
| [test/context/expected/frame/frame_62.json](/test/context/expected/frame/frame_62.json) | JSON | 104 | 0 | 0 | 104 |
| [test/include/frame_io.hpp](/test/include/frame_io.hpp) | C++ | 9 | 0 | 3 | 12 |
| [test/include/frame_meta.hpp](/test/include/frame_meta.hpp) | C++ | 14 | 0 | 3 | 17 |
| [test/include/io.hpp](/test/include/io.hpp) | C++ | 9 | 0 | 1 | 10 |
| [test/include/test.h](/test/include/test.h) | C++ | 12 | 0 | 3 | 15 |
| [test/lib/frame_io.cpp](/test/lib/frame_io.cpp) | C++ | 181 | 2 | 33 | 216 |
| [test/lib/test.c](/test/lib/test.c) | C | 59 | 0 | 15 | 74 |
| [test/src/color_codec/rgb_palette_codec_test.cpp](/test/src/color_codec/rgb_palette_codec_test.cpp) | C++ | 9 | 0 | 7 | 16 |
| [test/src/color_test.cpp](/test/src/color_test.cpp) | C++ | 231 | 8 | 30 | 269 |
| [test/src/encoder/provider/basic_block_test.cpp](/test/src/encoder/provider/basic_block_test.cpp) | C++ | 147 | 6 | 27 | 180 |
| [test/src/encoder/provider/len_reader_test.cpp](/test/src/encoder/provider/len_reader_test.cpp) | C++ | 35 | 0 | 6 | 41 |
| [test/src/math_test.cpp](/test/src/math_test.cpp) | C++ | 73 | 0 | 26 | 99 |
| [test/tool/analyse_decode/database.py](/test/tool/analyse_decode/database.py) | Python | 13 | 0 | 2 | 15 |
| [test/tool/analyse_decode/metric.cpp](/test/tool/analyse_decode/metric.cpp) | C++ | 105 | 2 | 22 | 129 |
| [test/tool/analyse_decode/sql_scripts/init.sql](/test/tool/analyse_decode/sql_scripts/init.sql) | MS SQL | 75 | 0 | 16 | 91 |
| [test/tool/color_test_helper.py](/test/tool/color_test_helper.py) | Python | 41 | 1 | 19 | 61 |
| [test/tool/fps_color.cpp](/test/tool/fps_color.cpp) | C++ | 161 | 14 | 47 | 222 |
| [test/tool/gen_frame/argument_generator.cpp](/test/tool/gen_frame/argument_generator.cpp) | C++ | 97 | 2 | 14 | 113 |
| [test/tool/gen_frame/frame_generator.cpp](/test/tool/gen_frame/frame_generator.cpp) | C++ | 120 | 2 | 10 | 132 |
| [test/tool/gen_frames.py](/test/tool/gen_frames.py) | Python | 14 | 0 | 1 | 15 |
| [test/tool/tgbi_helper.py](/test/tool/tgbi_helper.py) | Python | 7 | 0 | 1 | 8 |
| [tool/analyse_decode.py](/tool/analyse_decode.py) | Python | 0 | 0 | 1 | 1 |
| [tool/cmake_init.py](/tool/cmake_init.py) | Python | 20 | 0 | 4 | 24 |
| [tool/fps_color.py](/tool/fps_color.py) | Python | 27 | 0 | 4 | 31 |
| [tool/run.py](/tool/run.py) | Python | 14 | 0 | 2 | 16 |
| [tool/test.py](/tool/test.py) | Python | 14 | 0 | 2 | 16 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)