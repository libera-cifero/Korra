from ..deploy_manager import deploy_manager
import subprocess

class git_deploy_manager(deploy_manager):
    def _install(self, auto_yes:bool): 
        pass

    def _remove(self, auto_yes: bool):
        pass
    
    def install(self, auto_yes:bool):
        is_installed = subprocess.run(["git", "--version"]) == 0
        if is_installed:
            print("git is already installed!")
        else:
            self._install(auto_yes)

    def remove(self, auto_yes:bool):
        is_installed = subprocess.run(["git", "--version"]) == 0
        if not is_installed:
            print("git is already removed!")
        else:
            self._remove(auto_yes)