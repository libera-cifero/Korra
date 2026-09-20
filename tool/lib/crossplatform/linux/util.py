import shutil

def get_package_manager() -> str | None:
    """
    Check for apt, dnf, yum, pacman, zypper, apk, emerge, xbps-install or nix-env
    _
    """
    for cmd in ("apt", "dnf", "yum", "pacman", "zypper", "apk", "emerge", "xbps-install", "nix-env"):
        if shutil.which(cmd):
            return cmd
    return None