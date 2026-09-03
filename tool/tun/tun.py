#args: make|del MAKE_ARGS|DEL_ARGS
#MAKE_ARGS: ipv4_address/mask tun_name username
#DEL_ARGS: tun_name
import sys
import ipaddress
from manager import linux_tun_manager, windows_tun_manager, macos_tun_manager, android_tun_manager, ios_tun_manager

if len(sys.argv) < 2:
    print("Too less args!")

action = sys.argv[1]
if action not in ['make', 'del']:
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
    if len(sys.argv) < 5:
        print("Too few arguments!")
        exit(-3)

    net = ipaddress.IPv4Network(sys.argv[2], strict=False)
    address = ipaddress.IPv4Address(sys.argv[2].split('/')[0])
    name = sys.argv[3]
    user = sys.argv[4]
    manager.make_tun(address, net, name, user)

elif action == 'del':
    if len(sys.argv) < 3:
        print("Too few arguments!")
        exit(-4)

    manager.remove_tun(sys.argv[2])

