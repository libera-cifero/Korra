import sys
import os
import subprocess

class color:
   PURPLE = '\033[95m'
   CYAN = '\033[96m'
   DARKCYAN = '\033[36m'
   BLUE = '\033[94m'
   GREEN = '\033[92m'
   YELLOW = '\033[93m'
   RED = '\033[91m'
   BOLD = '\033[1m'
   UNDERLINE = '\033[4m'
   END = '\033[0m'

def get_binary_path(exe_name: str):
    if sys.platform == "win32":
        exe_name+=".exe"
    exe_path = os.path.join("bin", *exe_name.split('/'))
    return exe_path

def get_launch_cmd(exe_name: str):
    exe_path = get_binary_path(exe_name)
    launch_cmd = ""
    
    if sys.platform == "linux" or sys.platform == "darwin":
        launch_cmd = "./" + exe_path
    elif sys.platform == "win32":
        launch_cmd = exe_path
    else:
        raise Exception("Unknown OS!")
    return launch_cmd

def run_test(exe_name : str):
    launch_cmd = get_launch_cmd(exe_name)
    dirs = ["test", "context", "log"]
    log_path = os.path.join(*dirs, exe_name)
    p = ""
    for d in dirs:
        p = os.path.join(p, d)
        if not os.path.isdir(p):
            os.mkdir(p)
    print("\x1b[1;95mPLAIN TEST\x1b[0m")
    code = run_binary(exe_name, [])
    
    if code == 0:
        print("\x1b[1;95mMEMORY TEST\x1b[0m")
        process = subprocess.run(["valgrind", "--tool=memcheck", "--leak-check=full", "--quiet", f"--log-file={log_path}", launch_cmd])

def run_binary(exe_name : str, args : list[str]):
    file = get_binary_path(exe_name)
    result = subprocess.run([file, *args])
    return result.returncode
