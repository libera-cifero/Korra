from .spdlog_deploy_manager import spdlog_deploy_manager

class spdlog_linux_manager(spdlog_deploy_manager):
    def __init__(self):
        self.pckg_manager = get_package_manager()

        self.auto_yes_commands = {
            "install":{
                "apt": "apt install libspdlog-dev -y",
                "dnf": "dnf install libspdlog-dev -y",
                "yum": "yum install libspdlog-dev -y",
                "pacman": "pacman -S libspdlog-dev --noconfirm",
                "zypper": "zypper install -y libspdlog-dev",
                "apk": "apk add libspdlog-dev",
                "emerge": "emerge dev-build/libspdlog-dev",
                "xbps-install": "xbps-install -y libspdlog-dev",
                "nix-env": "nix-env -iA nixpkgs.libspdlog-dev"
            },
            "remove": {
                "apt": "apt purge libspdlog-dev -y",
                "dnf": "dnf remove libspdlog-dev -y",
                "yum": "yum remove libspdlog-dev -y",
                "pacman": "pacman -R libspdlog-dev --noconfirm",
                "zypper": "zypper remove -y libspdlog-dev",
                "apk": "apk del libspdlog-dev",
                "emerge": "emerge --unmerge dev-build/libspdlog-dev",
                "xbps-install": "xbps-remove -y libspdlog-dev",
                "nix-env": "nix-env -e libspdlog-dev"
            }
        }

        self.commands = {
            "install":
            {
                "apt": "apt install libspdlog-dev",
                "dnf": "dnf install libspdlog-dev",
                "yum": "yum install libspdlog-dev",
                "pacman": "pacman -S libspdlog-dev",
                "zypper": "zypper install libspdlog-dev",
                "apk": "apk add libspdlog-dev",
                "emerge": "emerge dev-build/libspdlog-dev",
                "xbps-install": "xbps-install libspdlog-dev",
                "nix-env": "nix-env -iA nixpkgs.libspdlog-dev"
            },
            "remove": {
                "apt": "apt purge libspdlog-dev",
                "dnf": "dnf remove libspdlog-dev",
                "yum": "yum remove libspdlog-dev",
                "pacman": "pacman -R libspdlog-dev",
                "zypper": "zypper remove libspdlog-dev",
                "apk": "apk del libspdlog-dev",
                "emerge": "emerge --unmerge dev-build/libspdlog-dev",
                "xbps-install": "xbps-remove libspdlog-dev",
                "nix-env": "nix-env -e libspdlog-dev"
            }
        }

    def install(self, auto_yes: bool): 
        print("spdlog library installing...")
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["install"][self.pckg_manager]
        print(command)
        success = subprocess.run(shlex.split(command)).returncode == 0
        if not success:
            print("spdlog library is not installed! Something went wrong :(")
            raise deploy_canceled_exception

        print("spdlog is installed successfully!")


    def remove(self, auto_yes: bool): 
        print("spdlog removing...")
        commands = self.auto_yes_commands if auto_yes else self.commands
        command = commands["remove"][self.pckg_manager]
        print(command)
        success = subprocess.run(shlex.split(command)).returncode == 0
        if not success:
            print("spdlog is not removed! Something went wrong :(")
            raise deploy_canceled_exception

        print("spdlog is removed successfully!")
