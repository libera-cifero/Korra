#!/usr/bin/bash
$root=$(pwd)
cd /usr/bin/korra
/usr/bin/python3 -m tool.tun.tun $*
cd $root