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

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
	if [ ! -d "$modpath" ]; then
		modpath=/kernel-tests/modules/lib/modules/$(uname -r)-dirty/extra
	fi
fi

spinlock_test_mod=$modpath/spinlock_test_module.ko
test_debugfs_dev=/sys/kernel/debug
spinlock_test_start=${test_debugfs_dev}/spinlock-test/start
spinlock_test_inner_time_path=${test_debugfs_dev}/spinlock-test/inner_test_time
spinlock_test_name=spinlock_test_module
print_banner="==========================================="
#Default value for Normal test
spinlock_test_iters=1
spinlock_test_inner_time=5

# Function spinlock_test_init
# Parameters:
#	No Parameters
# return 0 on success otherwise return 1
spinlock_test_init(){
	if [ ! -d $modpath ]; then
		echo "SPINLOCK: Couldn't find $modpath."
		return 1
	fi

	if [ ! -d $test_debugfs_dev ]; then
		mount -t debugfs nodev /sys/kernel/debug
		if [ $? -ne 0 ]; then
			echo "Couldn't mount debugfs."
			return 1
		fi
	fi

	if [ ! -d  /lib/modules/$(uname -r) ]; then
		mount -o remount rw, /
		mkdir -p /lib/modules/$(uname -r)
		return $?
	fi

	# remove spinlock_test_module before test
	lsmod | grep "$spinlock_test_name"
	if [ $? -eq 0 ]; then
		rmmod $spinlock_test_name > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "ERROR: failed to remove module $spinlock_test_name"
			return 1
		fi
	fi
	return 0
}

# Function spinlock_test_nominal
# Parameters:
# 1) number of iterations
# 2) spinlock test time
# return 0 on success otherwise return 1
spinlock_test(){
	# insert spinlock_test_module
	insmod $spinlock_test_mod
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to load module $spinlock_test_mod"
		return 1
	fi

	if [ ! -f ${spinlock_test_inner_time_path} ]; then
		echo "ERROR: failed to find ${spinlock_test_inner_time_path}"
		return 1
	fi

	if [ ! -f ${spinlock_test_start} ]; then
		echo "ERROR: failed to find ${spinlock_test_start}"
		return 1
	fi

	if [ "$1" -gt 0 ]; then
		spinlock_test_iters=$1
	fi

	if [ "$2" -gt 0 ]; then
		spinlock_test_inner_time=$2
	fi

	echo $spinlock_test_inner_time > ${spinlock_test_inner_time_path}
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to set ${spinlock_test_inner_time_path}!"
		return 1
	fi

	# Start to test
	echo $spinlock_test_iters > ${spinlock_test_start}
	if [ $? -ne 0 ]; then
		return 1
	fi

	# remove spinlock test module after test
	rmmod $spinlock_test_name > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to remove module $spinlock_test_name"
	fi

	return 0
}

while [ $# -gt 0 ]
do
    case $1 in
    -n | --nominal)
	spinlock_test_iters=1
	spinlock_test_inner_time=5
	break
    ;;
    -s | --stress)
    stress_test=1
	spinlock_test_iters=5
	spinlock_test_inner_time=10
    break
    ;;
    -c | --custom)
    custom_test=1
	if [ -n "$2" ]; then
		spinlock_test_iters=$2
	fi
	if [ -n "$3" ]; then
		spinlock_test_inner_time=$3
	fi
    break
    ;;
    -h | --help | *)
    echo "Usage: $0 [-n | --nominal] [-s | --stress] [(-c | --custom) <spinlock number> <test time>]"
	echo "	-n | --nominal: test 30s super spinlock bug "
	echo "	-s | --stress: open 4 spinlocks per core and total test 2 hours super spinlock bug."
	echo "	-c | --custom: open <spinlock number> spinlocks and test long time <test time> super spinlock bug"
    exit 1
    ;;
    esac
done

#init spinlock test
spinlock_test_init
if [ $? -ne 0 ]; then
	exit 1
fi

#do spinlock test
echo $print_banner
echo "Start spinlock Test"
echo "iterations: $spinlock_test_iters"
echo "test time per test: $spinlock_test_inner_time seconds"
echo $print_banner

spinlock_test $spinlock_test_iters $spinlock_test_inner_time

if [ $? -eq 0 ]; then
	echo -e "${print_banner}\nSPINLOCK Test Passed\n${print_banner}"
	exit 0
else
	echo -e "${print_banner}\nSPINLOCK Test Failed\n${print_banner}"
	exit 1
fi
