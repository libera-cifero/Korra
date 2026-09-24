from .cryptopp_deploy_manager import cryptopp_deploy_manager
from ..deploy_manager import deploy_canceled_exception
import subprocess
import shlex
import os
class cryptopp_linux_manager(cryptopp_deploy_manager):
    def __init__(self):
        self.ninja_commands = {
            True: {
                "install":{
                    "apt": "apt install ninja-build -y", #+
                    "dnf": "dnf install ninja-build -y",#+
                    "yum": "yum install ninja-build -y",
                    "pacman": "pacman -S ninja --noconfirm", #+
                    "zypper": "zypper install -y ninja",#+
                    "apk": "apk add ninja",#+
                    "emerge": "emerge dev-util/ninja",
                    "xbps-install": "xbps-install -y ninja",
                    "nix-env": "nix-env -i ninja"
                },
                "remove": {
                    "apt": "apt purge ninja -y",
                    "dnf": "dnf remove ninja -y",
                    "yum": "yum remove ninja -y",
                    "pacman": "pacman -R ninja --noconfirm",
                    "zypper": "zypper remove -y ninja",
                    "apk": "apk del ninja",
                    "emerge": "emerge --unmerge dev-build/ninja",
                    "xbps-install": "xbps-remove -y ninja",
                    "nix-env": "nix-env -e ninja"
                }
            },
            False: {
                "install": {
                    "apt": "apt install ninja-build",
                    "dnf": "dnf install ninja-build",
                    "yum": "yum install ninja-build",
                    "pacman": "pacman -S ninja",
                    "zypper": "zypper install ninja",
                    "apk": "apk add ninja",
                    "emerge": "emerge dev-build/ninja",
                    "xbps-install": "xbps-install ninja",
                    "nix-env": "nix-env -iA nixpkgs.ninja"
                },
                "remove": {
                    "apt": "apt purge ninja",
                    "dnf": "dnf remove ninja",
                    "yum": "yum remove ninja",
                    "pacman": "pacman -R ninja",
                    "zypper": "zypper remove ninja",
                    "apk": "apk del ninja",
                    "emerge": "emerge --unmerge dev-build/ninja",
                    "xbps-install": "xbps-remove ninja",
                    "nix-env": "nix-env -e ninja"
                }
            }
        }

    def install(self, auto_yes: bool):
        script = ""
        with open("/etc/korra/tool/data/deploy/install/linux/cryptopp.sh", 'r') as f:
            script = f.read()
        script = script.replace("$1", self.ninja_commands[auto_yes]["install"])
        print("cryptopp installing...")
        success = os.system(script) == 0
        if success:
            print("cryptopp installed successfully!")
        else:
            print("cryptopp isn't installed...")
            raise deploy_canceled_exception

    def remove(self, auto_yes: bool):
        pass