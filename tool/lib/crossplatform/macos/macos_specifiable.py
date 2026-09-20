from ..os_specifiable import os_specifiable
import sys
class macos_specifiable(os_specifiable):
    def is_usable(self) -> bool: 
        is_darwin = sys.platform == 'darwin'
        if is_darwin:
            try:
                import UIKit  #IOS-specificated module
                return False
            except ImportError:
                return True
        return False