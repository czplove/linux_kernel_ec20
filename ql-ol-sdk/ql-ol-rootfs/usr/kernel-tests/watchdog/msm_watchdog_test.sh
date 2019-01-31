# Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
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

watchdog_test_mod=${modpath}/msm_watchdog_test_module.ko

case "$1" in
    apps_wdog_bite|apps_wdog_bark|sec_wdog_bite|sec_wdog_scm|cpu_cntxt_test)
	thetest="$1"
	echo "Running test $thetest"
	;;
    *)
	echo "Unkown test: $1"
	echo "Supported tests: apps_wdog_bite apps_wdog_bark sec_wdog_bite sec_wdog_scm"
	exit 1
	;;
esac

if [ $1 = "cpu_cntxt_test" ]; then
	count=0
	cpupath="/sys/devices/system/cpu/cpu"
	cpuinfo="/proc/cpuinfo"
	cores=`grep -c "processor" $cpuinfo`
	stop mpdecision
	while [ $count -lt $cores ]
	do
		cpupath_enable=$cpupath$count"/online"
		echo 1 > $cpupath_enable
		count=$(( count + 1 ))
	done
fi

if [ -e $watchdog_test_mod ]; then
	insmod $watchdog_test_mod ${thetest}=1
else
	echo "watchdog test module not found (${watchdog_test_mod})"
fi

echo "Test failed"
exit 1
