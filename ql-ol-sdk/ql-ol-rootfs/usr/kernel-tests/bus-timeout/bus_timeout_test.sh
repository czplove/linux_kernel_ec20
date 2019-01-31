#! /bin/sh --

# Copyright (c) 2014, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#     * Neither the name of The Linux Foundation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
# ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

PLATFORM=`getprop ro.board.platform  | cut -b 4-7`

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
fi

if [ ! -f $modpath/bus_timeout_mod.ko ]; then
	echo "Bus timeout test: module not found\n"
	exit 1
fi

while [ $# -gt 0 ]
do
	case $1 in
	-c | --camera-timeout)
		insmod $modpath/bus_timeout_mod.ko bus_timeout_camera=1
		exit 1
	;;
	-u | --usb-timeout)
		insmod $modpath/bus_timeout_mod.ko bus_timeout_usb=1
		exit 1
	;;
	-p | --pc-save)
		insmod $modpath/bus_timeout_mod.ko pc_save=1
		exit 1
	;;
	-h | --help | *)
		echo "Usage: $0 [-c | --camera-timeout] [ -u | --usb-timeout] [-p | --pc-save] \\"
		echo "				[-h | --help]"
		echo "For example: $0 -c"
		exit 1
	;;
	esac
done
