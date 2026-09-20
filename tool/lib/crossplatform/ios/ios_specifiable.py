from ..os_specifiable import os_specifiable
import sys
class ios_specifiable(os_specifiable):
    def is_usable(self) -> bool: 
        is_darwin = sys.platform == 'darwin'
        if is_darwin:
            try:
                import UIKit  #IOS-specificated module
                return True
            except ImportError:
                return False
        return False