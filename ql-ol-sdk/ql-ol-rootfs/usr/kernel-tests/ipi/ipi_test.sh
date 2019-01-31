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



testpath=/data/kernel-tests

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
fi

# Mount debugfs
mount -t debugfs nodev /sys/kernel/debug 2>/dev/null

ipi_test_mod=${modpath}/ipi_test_module.ko
ipi_test_module_name=ipi_test_module
ipi_test_times="/sys/kernel/debug/ipi_test/test_times"
ipi_test_result="/sys/kernel/debug/ipi_test/test_result"

# Begin script execution here
# Sets 10000 as a default interation number of times to test IPI
test_times=10000

while [ $# -gt 0 ]
do
	case $1 in
	-n | --nominal)
		shift 1
		;;
	-a | --adversarial)
		shift 1
		;;
	-r | --repeatability)
		test_times=1000000
		shift 1
		;;
	-s | --stress)
		shift 1
		;;
	-v | --verbose)
		shift 1
		;;
	-t | --times)
	test_times=$2
	shift 2
	;;
	-h | --help | *)
	echo " Usage: $0 [(-t | --times) <test_number>] [-n | --nominal]"
	echo " test_number: the times number of ipi to test"
	exit 1
	;;
	esac
done

ipi_test_init(){
	# insert ipi_test_module
	insmod $ipi_test_mod
	if [ $? -ne 0 ] || [ ! -e $ipi_test_procfs ]; then
		echo "ERROR: failed to load module $ipi_test_mod"
		exit 1
	fi
}

ipi_test_exit(){
	#remove ipi_test_module after test
	lsmod | grep "$ipi_test_module_name" >> /dev/null
	if [ $? -eq 0 ]; then
		rmmod $ipi_test_module_name
		if [ $? -ne 0 ]; then
			echo "ERROR: failed to remove module $ipi_test_mod"
		fi
	fi
}

ipi_test(){
	ipi_test_init

	echo "IPI test starting"
	echo $test_times > ${ipi_test_times}
	test_result=`cat ${ipi_test_result}`

	ipi_test_exit

	if [ "$test_result" -eq "0" ]; then
		echo "Test passed"
		exit 0
	else
		echo "Test failed"
		exit 1
	fi
}

ipi_test
