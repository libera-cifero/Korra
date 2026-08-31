from manager.tun_manager import tun_manager
import sys, ipaddress
class windows_tun_manager(tun_manager):
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str): pass
    def remove_tun(self, tun_name:str): pass
    def is_usable(self) -> bool:
        return sys.platform == 'win32'