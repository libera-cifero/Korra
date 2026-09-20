from ..os_specifiable import os_specifiable
import sys
class windows_specifiable(os_specifiable):
    def is_usable(self) -> bool: 
        return sys.platform == 'win32'