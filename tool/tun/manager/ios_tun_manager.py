from .tun_manager import tun_manager
from ...lib.crossplatform.ios.ios_specifiable import ios_specifiable
import ipaddress

class ios_tun_manager(ios_specifiable, tun_manager):
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str): pass
    def remove_tun(self, tun_name:str): pass