from manager.tun_manager import tun_manager
from ...lib.crossplatform.windows.windows_specifiable import windows_specifiable
import ipaddress
class windows_tun_manager(windows_specifiable, tun_manager):
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str): pass
    def remove_tun(self, tun_name:str): pass