from .tun_manager import tun_manager
from ...lib.crossplatform.android.android_specifiable import android_specifiable
import ipaddress

class android_tun_manager(android_specifiable, tun_manager):
    def make_tun(self, address:ipaddress.IPv4Address, net:ipaddress.IPv4Network, tun_name:str, user:str): pass
    def remove_tun(self, tun_name:str): pass