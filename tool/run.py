import sys
import os
import subprocess

if len(sys.argv) < 2:
    print("Invalid argument count!")
    exit(-1)

exe_name = sys.argv[1]
if sys.platform == "win32":
    exe_name+=".exe"
exe_path = os.path.join("bin", exe_name)
result = subprocess.run([exe_path], capture_output=True, text=True)
print(result.stdout)
if result.stderr != "" and result.stderr != None:
    print(result.stderr)