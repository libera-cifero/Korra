import ipaddress
class tun_manager:
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str) -> None: pass
    def remove_tun(self, tun_name:str) -> None: pass
    def is_usable(self) -> bool: pass