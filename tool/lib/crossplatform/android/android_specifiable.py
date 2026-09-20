from ..os_specifiable import os_specifiable
import sys
class android_specifiable(os_specifiable):
    def is_usable(self) -> bool: 
        return sys.platform == 'android'