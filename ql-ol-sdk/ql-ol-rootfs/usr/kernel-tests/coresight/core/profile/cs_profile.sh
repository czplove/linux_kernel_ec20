#! /bin/sh

# Copyright (c) 2013, The Linux Foundation. All rights reserved.
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

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
fi

profile_mod=${modpath}/cs_profile_mod.ko

if [ ! -e $profile_mod ]; then
	echo "STM profile test module not found (${profile_mod})"
	exit 1
fi

source "$(dirname $0)/../cs_common.sh"
echo "-----Coresight STM Profile Test-----"
echo "------------------------------------"
source=`echo "$*" | sed -n 's/.*--source \(\w*\).*/\1/p'`
source=`echo $source | tr '[A-Z]' '[a-z]'`
if [[ "$source" ]]; then
	if [[ $source != "stm" ]]; then
		echo "STM is the required trace source. Exiting."
		exit 1
	fi
fi
#make etf current trace sink
echo 1 > $tmcetfpath"/curr_sink"
stm_enable
insmod $profile_mod stm_profile=1
if [ $? -ne 0 ]; then
	if [ -d /sys/module/cs_profile_mod ]; then
		echo 1 > /sys/module/cs_profile_mod/parameters/stm_profile
	fi
fi
stm_disable
echo "-----Coresight STM Profile Test Complete-----"
echo "---------------------------------------------"
echo ""
rmmod $profile_mod
