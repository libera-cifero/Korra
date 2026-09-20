from ..os_specifiable import os_specifiable
import sys
class linux_specifiable(os_specifiable):
    def is_usable(self) -> bool: 
        return sys.platform == 'linux'