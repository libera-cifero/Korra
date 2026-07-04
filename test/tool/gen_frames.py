import subprocess
import sys
import os

root = os.path.abspath('.')
runner = os.path.join(root, "tool", "run.py")
result = subprocess.run(["python3", runner, "tool/args_frame_gen_tool", sys.argv[1]], cwd = root, capture_output = True, text = True)
lines = result.stdout.split('\n')
for line in lines:
    if line=='': break
    args = line.split(' ')
    print(args)
    result = subprocess.run(["python3", runner, "tool/frame_gen_tool", *args], cwd = root)
