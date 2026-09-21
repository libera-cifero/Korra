from cmake_deploy_manager import cmake_deploy_manager
from ..deploy_manager import deploy_canceled_exception
from ....lib.crossplatform.linux.util import *
import subprocess
import shlex
import os

class cmake_linux_manager(cmake_deploy_manager):
    def __init__(self):
        self.pckg_manager = get_package_manager()
        self.auto_yes_commands = {
            "install":{
                "apt": "apt install cmake -y",
                "dnf": "dnf install cmake -y",
                "yum": "yum install cmake -y",
                "pacman": "pacman -S cmake --noconfirm",
                "zypper": "zypper install -y cmake",
                "apk": "apk add cmake",
                "emerge": "emerge dev-build/cmake",
                "xbps-install": "xbps-install -y cmake",
                "nix-env": "nix-env -iA nixpkgs.cmake"
            },
            "remove": {
                "apt": "apt purge cmake -y",
                "dnf": "dnf remove cmake -y",
                "yum": "yum remove cmake -y",
                "pacman": "pacman -R cmake --noconfirm",
                "zypper": "zypper remove -y cmake",
                "apk": "apk del cmake",
                "emerge": "emerge --unmerge dev-build/cmake",
                "xbps-install": "xbps-remove -y cmake",
                "nix-env": "nix-env -e cmake"
            }
        }

        self.commands = {
            "install":
            {
                "apt": "apt install cmake",
                "dnf": "dnf install cmake",
                "yum": "yum install cmake",
                "pacman": "pacman -S cmake",
                "zypper": "zypper install cmake",
                "apk": "apk add cmake",
                "emerge": "emerge dev-build/cmake",
                "xbps-install": "xbps-install cmake",
                "nix-env": "nix-env -iA nixpkgs.cmake"
            },
            "remove": {
                "apt": "apt purge cmake",
                "dnf": "dnf remove cmake",
                "yum": "yum remove cmake",
                "pacman": "pacman -R cmake",
                "zypper": "zypper remove cmake",
                "apk": "apk del cmake",
                "emerge": "emerge --unmerge dev-build/cmake",
                "xbps-install": "xbps-remove cmake",
                "nix-env": "nix-env -e cmake"
            }
        }

    def __append_kitware_repo(self):
        print("Appending kitware distro...")
        code = os.system("wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null")
        if code != 0:
            raise deploy_canceled_exception
        code = os.system("echo \"deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main\" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null")
        if code != 0:
            raise deploy_canceled_exception

    def __remove_kitrware_repo(self):
        pass

    def _install(self, auto_yes: bool):
        print("cmake installing...")
        if self.pckg_manager == None: 
            print("Sorry, installing cmake in your distro is unavailable :(")
            raise deploy_canceled_exception
        if self.pckg_manager == "apt": #if it is debian-based
            self.__append_kitware_repo()
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["install"][self.pckg_manager]
        print(command)
        result = subprocess.run(shlex.split(command))
        if result.returncode != 0:
            print("Something was went wrong...")
            raise deploy_canceled_exception
        
    def _update(self, auto_yes: bool):
        self._install(auto_yes)

    def _remove(self, auto_yes: bool):
        print("cmake removing...")
        if self.pckg_manager == None: 
            print("Sorry, removing cmake in your distro is unavailable :(")
            raise deploy_canceled_exception
        
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["remove"][self.pckg_manager]
        print(command)
        result = subprocess.run(shlex.split(command))
        if result.returncode != 0:
            print("Something was went wrong...")
            raise deploy_canceled_exception