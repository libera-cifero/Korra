from deploy_manager import *
from ...lib.crossplatform.linux.linux_specifiable import linux_specifiable
from .cmake.cmake_linux_manager import cmake_linux_manager
from .cryptopp.cryptopp_linux_manager import cryptopp_linux_manager
from .git.git_linux_manager import git_linux_manager
from .spdlog.splog_linux_manager import spdlog_linux_manager
import os
import subprocess
import re
from ...lib import path_util

class linux_manager(linux_specifiable, os_deploy_manager):
    def __init__(self):
        self.cmake = cmake_linux_manager()
        self.git = git_linux_manager()
        self.spdlog = spdlog_linux_manager()
        self.cryptopp = cryptopp_linux_manager()
        
    def install(self, auto_yes: bool):  
        pass
    
    def remove(self, auto_yes: bool): pass