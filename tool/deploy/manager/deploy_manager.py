from ....tool.lib.crossplatform.os_specifiable import os_specifiable

def input_yes_no(message: str, auto_yes:bool):
    print(message)
    if auto_yes: 
        print("[y/n]: y")
        return True
    result = ""
    while True:
        result = input("[y/n]: ")
        if len(result) < 1: continue
        c = result[0]
        if c in ['y','Y']:
            return True
        elif c in ['n', 'N']:
            return False

class deploy_canceled_exception(Exception): pass

class deploy_manager:
    def install(self, auto_yes: bool): pass
    def remove(self, auto_yes: bool): pass

class os_deploy_manager(os_specifiable, deploy_manager):
    pass