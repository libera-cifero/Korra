import os
import sys

def make_dir_if_not_exists(dir_path:list[str]):
    p = ""
    for d in dir_path:
        p = os.path.join(p, d)
        if not os.path.isdir(p):
            os.mkdir(p)

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