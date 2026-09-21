from ..deploy_manager import *
import os
import subprocess
import re

class check_version_status:
    CMAKE_NOT_FOUND = 0
    CMAKE_TOO_OLD = 1
    CMAKE_VALID = 2

class check_version_result:
    def __init__(self, status: int, min_version: str | None, current_version: str | None):
        self.status = status
        self.min_version = min_version
        self.current_version = current_version

    def is_not_found(self) -> bool: 
        return self.status == check_version_status.CMAKE_NOT_FOUND
    
    def is_too_old(self) -> bool:
        return self.status == check_version_status.CMAKE_TOO_OLD
    
    def is_valid(self) -> bool:
        return self.status == check_version_status.CMAKE_VALID

class cmake_deploy_manager(deploy_manager):
    def __read_minimum_required_version(self) -> str:
        cmake_version_pattern = r"\s*cmake_minimum_required\s*\(\s*VERSION\s+(\d+(?:\.\d+){2})\s*\)\s*"
        with open("CMakeLists.txt",'r') as f:
            text = f.read()
            match = re.search(cmake_version_pattern, text)
            if match is None:
                print("Invalid root CMakeLists.txt!", "cmake_minimum_required_version not found!")
                raise Exception()
            return match.groups()[0]

    def __compare_cmake_versions(self, version_a: str, version_b: str) -> int:
        a_vals = re.findall(r'\d+', version_a)
        b_vals = re.findall(r'\d+', version_b)
        for i in range(3):
            x_a = int(a_vals[i])
            x_b = int(b_vals[i])
            if x_a > x_b: return 1
            elif x_a < x_b: return -1
        return 0

    def __check_cmake_version(self) -> check_version_result:
        print("Checking cmake version...")
        result = subprocess.run(["cmake", "--version"], capture_output=True, text=True)
        if result.returncode != 0:
            return check_version_result(check_version_status.CMAKE_NOT_FOUND, None, None)
        pattern = r"cmake version (\d+(?:\.\d+){2})"
        match = re.search(pattern, result.stdout)
        if match is None:
            print("\"cmake --version\" had returned invalid result!", result.stdout)
            raise Exception()
        
        version = match.group(0)
        min_version = self.__read_minimum_required_version()
        is_normal_version = self.__compare_cmake_versions(version, min_version) >= 0
        status = check_version_status.CMAKE_VALID if is_normal_version else check_version_status.CMAKE_TOO_OLD 
        return check_version_result(status, min_version, version)

    def _install(self, auto_yes: bool): pass

    def _update(self, auto_yes: bool): pass

    def install(self, auto_yes: bool):
        print("Tying to install cmake...")
        check_res = self.__check_cmake_version()
        if check_res.is_valid():
            print(f"Cmake is already installed!\nMin version: {check_res.min_version}\nCurrent version: {check_res.current_version}")
            return
        elif check_res.is_not_found():
            do_install = input_yes_no("cmake not found. Do you wanna install it?", auto_yes)
            if do_install:
                self._install(auto_yes)
            else:
                raise deploy_canceled_exception
        elif check_res.is_too_old():
            print("cmake is depreacated",f"Min version: {check_res.min_version}",f"Current version: {check_res.current_version}")
            do_update = input_yes_no("Do you wanna update it?", auto_yes)
            if do_update:
                self._update(auto_yes)
            else:
                raise deploy_canceled_exception
        print("cmake is installed successfully!")

    def _remove(self, auto_yes: bool): pass

    def remove(self, auto_yes: bool):
        result = subprocess.run(["cmake", "--version"], capture_output=True, text=True)
        if result.returncode != 0:
            print("cmake is already removed!")
            return
        self._remove(auto_yes)