from deploy_manager import *
from ...lib.crossplatform.linux.linux_specifiable import linux_specifiable
import os
import subprocess
import re
from ...lib import path_util


class linux_manager(linux_specifiable, os_deploy_manager):
    def install(self, auto_yes: bool): pass
    def remove(self, auto_yes: bool): pass