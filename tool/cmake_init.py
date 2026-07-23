import os
from sys import argv
import subprocess

if len(argv) < 2:
    print("Invalid argument count!")
    exit(-1)


build_path = os.path.join("build")
if not os.path.exists(build_path): os.mkdir(build_path)

build_type = argv[1]
if build_type not in ["Debug", "Release"]:
    print(f"Invalid build type {build_type}! Expected Debug or Release!")
    exit(-3)

build_type_flag = f"-DCMAKE_BUILD_TYPE={build_type}"
subprocess.run(["cmake", "-S", ".", "-B", build_path, build_type_flag])