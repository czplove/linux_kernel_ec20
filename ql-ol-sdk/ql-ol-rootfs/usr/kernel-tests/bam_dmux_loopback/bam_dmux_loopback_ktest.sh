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

[ -d /system/lib/modules ] && modpath=/system/lib/modules
[ -d /kernel-tests/modules/lib/modules/$(uname -r 2>/dev/null)/extra ] && modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
[ -d /usr/kernel-tests/bam_dmux_loopback ] && modpath=/usr/kernel-tests/bam_dmux_loopback

if [[ -z "$modpath" ]]; then
        echo "Couldn't find a path to the kernel module. Bailing."
        exit 1
fi

bam_dmux_loopback_ktest_mod=${modpath}/bam_dmux_loopback.ko

load_sub() {
	insmod $bam_dmux_loopback_ktest_mod

	if [ $? -ne 0 ]; then
		echo "ERROR: Failed to load module $bam_dmux_loopback_ktest_mod"
		exit
	fi

	# The DLKM will take a few seconds to initialize itself and reinit
	# bam_dmux.  Any test that happens to run during that time may fail.
	sleep 30

	export PATH=$PATH:/data/busybox/
	chmod 744 /data/busybox/busybox
	/data/busybox/busybox --install
	ifconfig rmnet0 192.168.1.2
	stop mpdecision
	echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
}

unload_sub() {
	#rmmod $bam_dmux_loopback_ktest_mod
	start mpdecision
	echo ondemand > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
}

#this subroutine runs ping and accepts 4 arguments
#the first argument is the maximum allowed average latency in milliseconds of the ping
#the second argument is the interval between pings in seconds, the -i argument to ping
#the third argument is the -c flag to ping
#the fourth argument is the number of times the test should be run
ping_sub() {

	rc=0
	maxlatency=$1
	interval=$2
	count=$3
	iterations=$4

	for i in {1..$iterations}
	do
		average=`ping -c$count -i$interval 192.168.1.3 | grep -o /[0-9][0-9]*\.[0-9][0-9]* | head -1 | sed 's:/::'`
		greaterthanmax=$(awk 'BEGIN{ print ('$maxlatency' < '$average')}')
		if [ "$greaterthanmax" -eq 1 ]; then
			echo "Test failed"
			unload_sub
			exit 1
		fi
	done
	echo "Test passed"
}

load_sub

for param in $*
do
	case $param in
		-h | --help)
		echo "Usage: $0 [-n | --nominal] [-a | --adversarial] [-r | --repeatability] [-s | --stress] [-v | --verbose]"
			break
			;;
		-v | --verbose)
			echo "not implemented"
			;;
		-a | --adversarial)
			echo "not implemented"
			rc=1
			break
			;;
		-n | --nominal)
			ping_sub 10 1 5 1
			break
			;;
		-r | --repeatability)
			ping_sub 10 1 5 10
			break
			;;
		-s | --stress)
			ping_sub 10 .2 100 1
			break
			;;
		*)
			echo "Invalid parameter"
			rc=1
			break
			;;
	esac
done

unload_sub

exit $rc
