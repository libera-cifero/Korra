#args: ipv4_address/mask tun_name
import sys
import ipaddress
import os
import subprocess
import re
import shlex
import getpass

def make_linux_tun_ip(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str) -> None:
    a = f"ip tuntap add dev {name} mode tun user {getpass.getuser()}"
    b = f"ip link set {name} up"
    c = f"ip addr add {address}/{net.prefixlen} dev {name}"
    print(a)
    res = subprocess.run(shlex.split(a))
    if res.returncode != 0: return
    print(b)
    res = subprocess.run(shlex.split(b))
    if res.returncode != 0: return
    print(c)
    res = subprocess.run(shlex.split(c))
    if res.returncode != 0: return
    
def make_linux_tun_ifconfig(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str) -> None:
    pass

def make_linux_tun(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str) -> None:
    result = subprocess.run(["which", "ip"], capture_output=True)
    path = str(result.stdout.splitlines()[0])
    if re.match('which: \\.+', path):
        return
    if len(path) > 0:
        make_linux_tun_ip(address, net, name)
        return
    result = subprocess.run(["which", "ifconfig"], capture_output=True)
    path = str(result.stdout.splitlines()[0])
    if len(path) > 0:
        make_linux_tun_ifconfig(address, net, name)
    else:
        print("Making a tun is impossible. Try to install ip or ifconfig\nsudo apt install ip\nOR\nsudo apt install ifconfig")

def make_windows_tun(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str):
    pass

def make_macos_tun(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str):
    pass

def make_android_tun(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str):
    pass

def make_ios_tun(address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, name:str):
    pass

if len(sys.argv) < 3:
    print("Too less args!")

net = ipaddress.IPv4Network(sys.argv[1], strict=False)
address = ipaddress.IPv4Address(sys.argv[1].split('/')[0])
name = sys.argv[2]

actions = {
    'linux': make_linux_tun,
    'win32': make_windows_tun,
    'darwin': make_macos_tun,
    'android': make_android_tun,
    'ios': make_ios_tun
}

#to do: upgrade it later for android and IOS. Android's program will launch using termux
platform = sys.platform 
if platform not in actions:
    print(f"Unsupported OS {platform}")
    exit(1)

make_tun = actions[platform]
make_tun(address, net, name)