#args: make|del|info MAKE_ARGS|DEL_ARGS|INFO_ARGS
#MAKE_ARGS: ipv4_address/mask tun_name username
#DEL_ARGS: tun_name
#INFO_ARGS: INFO.ADDRESS_ARGS|INFO.NAME_ARGS
#INFO.ADDRESS_ARGS: --address|-a ipv4_address
#INFO.NAME_ARGS: --name|-n tun_name

import sys
import ipaddress
from .manager import linux_tun_manager, windows_tun_manager, macos_tun_manager, android_tun_manager, ios_tun_manager

argv = sys.argv

if len(argv) < 2:
    print("Too less args!")

action = argv[1]
if action not in ['make', 'del', 'info']:
    print(f"Unsuppored action {action}! make and del are availble only!")
    exit(-1)

managers = [
    linux_tun_manager.linux_tun_manager(), 
    windows_tun_manager.windows_tun_manager(), 
    macos_tun_manager.macos_tun_manager(),
    android_tun_manager.android_tun_manager(),
    ios_tun_manager.ios_tun_manager()
]

manager = None
for mgr in managers:
    if mgr.is_usable():
        manager = mgr
        break

if manager == None:
    print("Unsupported OS")
    exit(-2)

if action == 'make':
    if len(argv) < 5:
        print("Too few arguments!")
        exit(-3)

    net = ipaddress.IPv4Network(argv[2], strict=False)
    address = ipaddress.IPv4Address(argv[2].split('/')[0])
    name = argv[3]
    user = argv[4]
    manager.make_tun(address, net, name, user)

elif action == 'del':
    if len(argv) < 3:
        print("Too few arguments!")
        exit(-4)

    manager.remove_tun(argv[2])

elif action == 'info':
    if len(argv) < 4: #korra-tun info --address 10.12.34.56
        print("Too few args")
        exit(-5)
    info_type = argv[2]
    if info_type in ['--address', '-a']:
        info = manager.get_info_by_ip(ipaddress.ip_address(argv[3]))
    elif info_type in ['--name', '-n']:
        info = manager.get_info_by_name(argv[3])
    else:
        print(f"Invalid info type \"{info_type}\"! --address, -a or --name, -n are available!")
        exit(-6)
    
    print(f"NAME={info.name}\nNET={info.net}\nADDRESS={info.address}\nMTU={info.mtu}\nUSER={info.user}")