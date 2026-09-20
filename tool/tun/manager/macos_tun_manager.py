from manager.tun_manager import tun_manager
from ...lib.crossplatform.macos.macos_specifiable import macos_specifiable
import sys, ipaddress
class macos_tun_manager(macos_specifiable, tun_manager):
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str): pass
    def remove_tun(self, tun_name:str): pass