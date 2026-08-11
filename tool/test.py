import os
import subprocess
import lib
import sys
import re

def get_test_targets():
    result = subprocess.run(
        ["cmake", "--build", "build", "--target", "help"],
        capture_output=True, text=True
    )

    lines = result.stdout.splitlines()
    tests = []
    for line in lines:
        match = re.search("\\w+_test$", line)
        if match:
            tests.append(match[0])
    
    return tests

def build_tests(targets):
    subprocess.run([ "cmake", "--build", "build", "--target", *targets ])

def run_tests(tests):
    for test in tests: 
        lib.run_test(test)

if len(sys.argv) < 2:
    tests = get_test_targets()
    build_tests(tests)
    run_tests(tests)
else:
    run_tests(sys.argv[1::])