import os
import subprocess

bin_dir = os.path.join("..","bin")

files=[]
for f in os.listdir(bin_dir):
    file_path = os.path.join(bin_dir, f)
    if os.path.isfile(file_path) and (file_path.endswith("_test") or file_path.endswith("_test.exe")):
        files.append(file_path)

for file in files:
    file_path = os.path.join(bin_dir, f)
    result = subprocess.run([file_path], capture_output=True, text=True)
    print(result.stdout)
    if result.stderr != "" and result.stderr != None:
        print(result.stderr)
