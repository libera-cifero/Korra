from .git_deploy_manager import git_deploy_manager
from ..deploy_manager import deploy_canceled_exception
from ....lib.crossplatform.linux.util import get_package_manager
import subprocess
import shlex

class git_linux_manager(git_deploy_manager):
    def __init__(self):
        self.pckg_manager = get_package_manager()
        self.auto_yes_commands = {
            "install":{
                "apt": "apt install git -y",
                "dnf": "dnf install git -y",
                "yum": "yum install git -y",
                "pacman": "pacman -S git --noconfirm",
                "zypper": "zypper install -y git",
                "apk": "apk add git",
                "emerge": "emerge dev-build/git",
                "xbps-install": "xbps-install -y git",
                "nix-env": "nix-env -iA nixpkgs.git"
            },
            "remove": {
                "apt": "apt purge git -y",
                "dnf": "dnf remove git -y",
                "yum": "yum remove git -y",
                "pacman": "pacman -R git --noconfirm",
                "zypper": "zypper remove -y git",
                "apk": "apk del git",
                "emerge": "emerge --unmerge dev-build/git",
                "xbps-install": "xbps-remove -y git",
                "nix-env": "nix-env -e git"
            }
        }

        self.commands = {
            "install":
            {
                "apt": "apt install git",
                "dnf": "dnf install git",
                "yum": "yum install git",
                "pacman": "pacman -S git",
                "zypper": "zypper install git",
                "apk": "apk add git",
                "emerge": "emerge dev-build/git",
                "xbps-install": "xbps-install git",
                "nix-env": "nix-env -iA nixpkgs.git"
            },
            "remove": {
                "apt": "apt purge git",
                "dnf": "dnf remove git",
                "yum": "yum remove git",
                "pacman": "pacman -R git",
                "zypper": "zypper remove git",
                "apk": "apk del git",
                "emerge": "emerge --unmerge dev-build/git",
                "xbps-install": "xbps-remove git",
                "nix-env": "nix-env -e git"
            }
        }

    def _install(self, auto_yes:bool):
        print("git installing...")
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["install"][self.pckg_manager]
        print(command)
        success = subprocess.run(shlex.split(command)).returncode == 0
        if not success:
            print("git is not installed! Something went wrong :(")
            raise deploy_canceled_exception

        print("git is installed successfully!")

    def _remove(self, auto_yes: bool):
        print("git removing...")
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["remove"][self.pckg_manager]
        print(command)
        success = subprocess.run(shlex.split(command)).returncode == 0
        if not success:
            print("git is not removed! Something went wrong :(")
            raise deploy_canceled_exception

        print("git is removed successfully!")
