import os
import subprocess

files = [
    f for f in os.listdir("bin")
    if os.path.isfile(os.path.join("bin", f)) and str(f).endswith("_test")
]

for file in files:
    result = subprocess.run([f"bin/{file}"], capture_output=True, text=True)
    print(result.stdout)
    if result.stderr != "" and result.stderr != None:
        print(result.stderr)
