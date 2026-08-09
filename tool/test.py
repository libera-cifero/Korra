import os
import subprocess
import lib
import sys

if len(sys.argv) < 2:
    bin_dir = os.path.abspath(os.path.join('.','bin'))
    print(bin_dir)
    files=[]
    for f in os.listdir(bin_dir):
        file_path = os.path.join(bin_dir, f)
        if os.path.isfile(file_path) and (file_path.endswith("_test") or file_path.endswith("_test.exe")):
            files.append(f)

    for file in files: lib.run_test(file)
else:
    tests = sys.argv[1::]
    for test in tests:
        lib.run_test(test)