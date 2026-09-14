import ipaddress

class TunInfoNotFoundException(Exception): pass

class tun_info:
    def __init__(self, name:str, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, mtu: int, user:str):
        self.net = net
        self.mtu = mtu
        self.address = address
        self.name = name
        self.user = user

class tun_manager:
    def make_tun(self, address: ipaddress.IPv4Address, net: ipaddress.IPv4Network, tun_name:str, user:str) -> None: pass
    def remove_tun(self, tun_name:str) -> None: pass
    def is_usable(self) -> bool: pass
    def get_info_by_name(self, tun_name:str) -> tun_info | None: pass
    def get_info_by_ip(self, ip: ipaddress.IPv4Address) -> tun_info: pass