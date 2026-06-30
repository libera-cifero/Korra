import os
from sys import argv
import subprocess

if len(argv) < 3:
    print("Invalid argument count!")
    exit(-1)
project = argv[1]
if project not in ['src', 'test']:
    print(f"Invalid project {project}! Expected src or test!")
    exit(-2)

build_path = os.path.join("build")
cmake_cache_dir = os.path.join(build_path, project)
if not os.path.exists(build_path): os.mkdir(build_path)
if not os.path.exists(cmake_cache_dir): os.mkdir(cmake_cache_dir)

build_type = argv[2]
if build_type not in ["Debug", "Release"]:
    print(f"Invalid build type {build_type}! Expected Debug or Release!")
    exit(-3)

build_type_flag = f"-DCMAKE_BUILD_TYPE={build_type}"
subprocess.run(["cmake", "-S", project, "-B", cmake_cache_dir, build_type_flag])