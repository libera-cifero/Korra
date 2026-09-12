import sys
import os
import subprocess
import lib

if len(sys.argv) < 2:
    print("Invalid argument count!")
    exit(-1)
args = sys.argv[2:]
lib.runner.run_binary(sys.argv[1], args)