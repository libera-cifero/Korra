from manager import tun_manager
from ...lib.crossplatform.linux.linux_specifiable import linux_specifiable
import os, sys, subprocess, shutil, ipaddress, shlex
import re
import pwd

class linux_tun_manager(linux_specifiable, tun_manager.tun_manager):
    def __init__(self):
        self.__tun_info_pattern = "\\d+: ([^\\s]+):.+mtu (\\d+).+inet (\\d{1,3}(?:\\.\\d{1,3}){3})\\/(\\d+).+inet6 ([0-f:]+)\\/(\\d+)"

    def __tun_name_to_service_path(self, tun_name:str) -> str:
        return f"/etc/systemd/system/korra-tun-{tun_name}.service"

    def __autostart_made(self, tun_name:str) -> bool:
        service_path = self.__tun_name_to_service_path(tun_name)
        return os.path.isfile(service_path)
        
    def __make_systemd_autostart(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str) -> None:
        service_path = self.__tun_name_to_service_path(tun_name)
        cmd = f"/usr/bin/korra-tun {address}/{net.prefixlen} {tun_name} {user}"
        text = ""
        with open("/etc/korra/linux_template.service", "r") as file: 
            text = file.read()
            text = text.replace("$address", str(address)).replace("$mask", str(net.prefixlen)).replace("$tun_name",tun_name).replace("$user", user)
        with open(service_path, "w") as f: 
            f.write(text)

    def __remove_systemd_autostart(self, tun_name:str) -> None:
        service_path = self.__tun_name_to_service_path(tun_name)
        if os.path.isfile(service_path):
            os.remove(service_path)

    def __execute_command_chain(self, commands:list[str]) -> int:
        for cmd in commands:
            print(cmd)
            res = subprocess.run(shlex.split(cmd))
            ret = res.returncode
            if ret != 0:
                return ret
        return 0

    def __get_package_manager(self) -> str | None:
        for cmd in ("apt", "dnf", "yum", "pacman", "zypper", "apk", "emerge", "xbps-install", "nix-env"):
            if shutil.which(cmd):
                return cmd
        return None

    def __get_installation_command(self)-> str | None:
        pckg_manager = self.__get_package_manager()
        cmd_map = {
            "apt": "sudo apt update\nsudo apt install iproute2",
            "dnf": "sudo dnf install iproute",
            "yum": "sudo yum install iproute",
            "pacman": "sudo pacman -Sy iproute2",
            "zypper": "sudo zypper install iproute2",
            "apk": "sudo apk add iproute2",
            "emerge": "sudo emerge sys-apps/iproute2",
            "xbps-install": "sudo xbps-install -S iproute2",
            "nix-env": "nix-env -iA nixpkgs.iproute2",
            None: None
        }

        return cmd_map[pckg_manager]

    def __show_ip_installation_command(self) -> str | None:
        print("ip utility is not found")
        cmd = self.__get_installation_command()
        if cmd != None:
            print(f"Try to install using\n\n{cmd}")
        else:
            print("Package manager is not found too")

    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str) -> None:
        if shutil.which("ip"):
            self.__execute_command_chain([
                f"ip tuntap add dev {tun_name} mode tun user {user}", 
                f"ip link set {tun_name} up",
                f"ip addr add {address}/{net.prefixlen} dev {tun_name}"
            ])
        else:
            self.__show_ip_installation_command()
            return
        if not self.__autostart_made(tun_name):
            self.__make_systemd_autostart(address, net, tun_name, user)
            self.__execute_command_chain([
                "systemctl daemon-reload",
                f"systemctl enable korra-tun-{tun_name}"
            ])

    def remove_tun(self, tun_name:str) -> None:
        if shutil.which("ip"):
            self.__execute_command_chain([
                f"ip link del {tun_name}"
            ])
        else:
            self.__show_ip_installation_command()
            return

        self.__remove_systemd_autostart(tun_name)
        self.__execute_command_chain(["systemctl daemon-reload"])

    def __get_username(self, tun_name:str) -> str:
        result = subprocess.run(["ip", "tuntap", "show"], capture_output=True, text=True)
        if result.returncode != 0:
            raise Exception("Error when executed command \"ip tuntap show\"!")
        pattern = f"{tun_name}:.+user (\\d+)"
        match = re.search(pattern, result.stdout)
        if match is None:
            raise Exception(f"Tried to get user id\nInvalid pattern {pattern} for output\n{result.stdout}")
        
        user_id = int(match.group(1))
        return pwd.getpwuid(user_id).pw_name

    def __match_to_tun_info(self, match: re.Match):
        groups = match.groups()
        return tun_manager.tun_info(
            name = groups[0], 
            mtu = int(groups[1]),
            address = ipaddress.IPv4Address(groups[2]), 
            net = ipaddress.IPv4Network(int(groups[3])),
            user = None
        )

    def get_info_by_name(self, tun_name:str) -> tun_manager.tun_info: 
        cmd = f"ip addr show {tun_name}"
        result = subprocess.run(["ip", "addr", "show", tun_name], capture_output = True)
        if result.returncode != 0:
            raise tun_manager.TunInfoNotFoundException(f"Tun interface \"{tun_name}\" not found or execution error!")
        pattern = self.__tun_info_pattern
        match = re.match(pattern, text)
        if match is None:
            raise Exception(f"Tried to get general TUN info\nInvalid pattern {pattern} for output\n{text}")
        info = self.__match_to_tun_info(match)
        info.user = self.__get_username(info.tun_name)
        return info
    
    def get_info_by_ip(self, ip: ipaddress.IPv4Address) -> tun_manager.tun_info: 
        cmd = f"ip addr show {tun_name}"
        result = subprocess.run(["ip", "addr", "show"], capture_output = True)
        pattern:str = self.__tun_info_pattern
        entries:list[re.Match] = re.findall(pattern, result.stdout)
        for match in entries:
            info = self.__match_to_tun_info(match)
            if ip in info.net:
                info.user = self.__get_username(info.name)
                return info
        raise tun_manager.TunInfoNotFoundException(f"Tun interface for ip {ip} not found!")