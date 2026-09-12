import sys
import os
import subprocess
from lib.test_result import test_result, test_type
from lib.path_util import *

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

def run_plain_test(exe_name:str, log_path:str, result_file: test_result) -> bool:
    print("\x1b[1;95mPLAIN TEST\x1b[0m")
    print(f"Result of test is placed in {os.path.abspath(log_path)}")
    passed = run_binary(exe_name, [], log_path) == 0
    result_file.mark_test(exe_name, test_type.PLAIN, log_path, passed)
    return passed

def run_memory_test(exe_name:str, valgrind_log_path:os.PathLike, result_file: test_result) -> bool:
    launch_cmd = get_launch_cmd(exe_name)
    print("\x1b[1;95mMEMORY TEST\x1b[0m")
    print(f"For memory leaking check {valgrind_log_path}")
    process = subprocess.run(["valgrind", "--tool=memcheck", "--leak-check=full", "--quiet", "--track-origins=yes" ,f"--log-file={valgrind_log_path}", launch_cmd])
    valgrind_passed = False
    with open(valgrind_log_path, 'r') as v:
        content = v.read()
        valgrind_passed = not content.strip() or len(content) == 0
    result_file.mark_test(exe_name, test_type.MEMORY, valgrind_log_path, valgrind_passed)
    return valgrind_passed

def __print_memory_test_status(passed:bool):
    msg = ""
    if passed:
        msg = "\x1b[1;92mMEMORY PASSED\x1b[0m"
    else:
        msg = "\x1b[1;91mMEMORY FAILED\x1b[0m"
    print(msg + '\n')

def run_test(exe_name: str, result_file: test_result):
    launch_cmd = get_launch_cmd(exe_name)
    dirs = ["test", "context", "log"]
    log_path = os.path.abspath(os.path.join(*dirs, exe_name+'.log'))
    make_dir_if_not_exists(dirs)
    print(f"\x1b[1m{exe_name}\x1b[0m")
    
    plain_passed = run_plain_test(exe_name, log_path, result_file)
    if plain_passed:
        valgrind_log_path = os.path.abspath(os.path.join(*dirs, exe_name+'.valgrind'))
        print("\x1b[1;92mPASSED\x1b[0m")
        memory_passed = run_memory_test(exe_name, valgrind_log_path, result_file)
        __print_memory_test_status(memory_passed)
    else:
        print("\x1b[1;91mTEST FAILED\x1b[0m\n")
        
def run_binary(exe_name : str, args : list[str], filename_stdout:str | None = None):
    file = get_binary_path(exe_name)
    if filename_stdout == None:
        result = subprocess.run([file, *args])
    else:
        with open(filename_stdout, 'w') as f:
            result = result = subprocess.run([file, *args], stdout=f)
    return result.returncode