from cmake_deploy_manager import cmake_deploy_manager
from ..deploy_manager import deploy_canceled_exception
from ....lib.crossplatform.linux.util import *
import subprocess
import shlex

class cmake_linux_manager(cmake_deploy_manager):
    def _install(self, auto_yes: bool):
        pckg = get_package_manager()
        commands = {
            "apt": "apt install cmake -y", 
            "dnf": "", 
            "yum": "", 
            "pacman": "", 
            "zypper": "", 
            "apk": "", 
            "emerge": "", 
            "xbps-install": "",
            "nix-env": ""
        }
        if pckg == None: 
            print("Sorry, installing cmake in your distro is unavailable :(")
            raise deploy_canceled_exception
        command = commands[pckg]
        print(command)
        result = subprocess.run(shlex.split(command))
        if result.returncode != 0:
            print("Something was went wrong...")
            raise deploy_canceled_exception
        
    def _update(self, auto_yes: bool): pass

    def _remove(self, auto_yes: bool): pass