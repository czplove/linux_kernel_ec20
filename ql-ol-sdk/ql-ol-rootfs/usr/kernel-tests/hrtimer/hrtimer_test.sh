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

[ -d /system/lib/modules/ ] && modpath=/system/lib/modules
[ -d /kernel-tests/modules/lib/modules/$(uname -r 2>/dev/null)/extra ] && modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
[ -d /usr/kernel-tests/hrtimer ] && modpath=/usr/kernel-tests/hrtimer

if [[ -z "$modpath" ]]; then
	echo "Couldn't find a path to kernel module. Bailing..."
	exit 1
fi

hrtimer_test_mod=$modpath/hrtimer_test_module.ko
test_debugfs_dev=/sys/kernel/debug
hrtimer_dev_sys_result=${test_debugfs_dev}/hrtimer-test/status
hrtimer_dev_sys_count=${test_debugfs_dev}/hrtimer-test/count
hrtimer_dev_sys_timernum=${test_debugfs_dev}/hrtimer-test/timer_num
hrtimer_dev_sys_start=${test_debugfs_dev}/hrtimer-test/start
hrtimer_module_name=hrtimer_test_module
hrtimer_test_time=3600
timer_num=0
count=0

# Function hrtimer_test_init
# Parameters:
# return 0 on success otherwise return 1
hrtimer_test_init(){
	if [ ! -d $test_debugfs_dev ]; then
		mount -t debugfs nodev /sys/kernel/debug
		if [ $? -ne 0 ]; then
			echo "HRTIMER: Couldn't mount debugfs."
			return 1
		fi
	fi

	if [ ! -d $modpath ]; then
		echo "HRTIMER: Couldn't find a path to the kernel module."
		return 1
	fi

	if [ ! -d  /lib/modules/$(uname -r) ]; then
		mount -o remount rw, /
		mkdir -p /lib/modules/$(uname -r)
		return $?
	fi

	# remove hrtimer_test_module before test
	lsmod | grep "$hrtimer_module_name"
	if [ $? -eq 0 ]; then
		rmmod $hrtimer_module_name > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "ERROR: failed to remove module $hrtimer_test_mod"
			return 1
		fi
	fi
	return 0
}

# Function hrtimer_test
# Parameters:
# 1) Timer test loop for each thread
# 2) The number of timer to open
# return 0 on success otherwise return 1
hrtimer_test(){
	# insert hrtimer_test_module
	insmod $hrtimer_test_mod
	if [ $? -ne 0 ] || [ ! -e $hrtimer_test_dev_sys ]; then
		echo "ERROR: failed to load module $hrtimer_test_mod"
		return 1
	fi

	if [ -z "$1" ]; then
		if [ -f $hrtimer_dev_sys_count ]; then
			count=`cat ${hrtimer_dev_sys_count}`
		else
			echo "ERROR: failed to get hrtimer test times"
			return 1
		fi
	else
		count=$1
	fi

	if [ -z "$2" ]; then
		if [ -f $hrtimer_dev_sys_timernum ]; then
			timer_num=`cat ${hrtimer_dev_sys_timernum}`
		else
			echo "ERROR: failed to get the number of timer"
			return 1
		fi
	else
		timer_num=$2
	fi

	# Start to test
	echo "Hrtimer is starting. It might take several minutes or longer..."
	echo $count > ${hrtimer_dev_sys_count}
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to pass parameter(count) to driver"
		return 1
	fi

	echo $timer_num > ${hrtimer_dev_sys_timernum}
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to pass parameter(timer_num) to driver"
		return 1
	fi

	if [ -f ${hrtimer_dev_sys_start} ]; then
		echo 1 > ${hrtimer_dev_sys_start}
		if [ $? -ne 0 ]; then
			echo "ERROR: failed to start test"
			return 1
		fi
	else
		echo "ERROR: failed to find the path: $hrtimer_dev_sys_start"
		return 1
	fi
	# check test result
	while [ $hrtimer_test_time -gt 0 ]; do
		let hrtimer_test_time=hrtimer_test_time-1
		sleep 1
		start_result=`cat $hrtimer_dev_sys_start`
		if [ ${start_result} -eq 0 ]; then
			break
		fi
	done

	status=`cat $hrtimer_dev_sys_result`
	if [ $status -ne 0 ]; then
		return 1
	else
		return 0
	fi
}

# Function hrtimer_test_nominal
# Parameters:
# return 0 on success otherwise return 1
hrtimer_nominal_test(){
	hrtimer_test # default timer number:2 test loop: 1000
	if [ $? -ne 0 ]; then
		return 1
	fi
	return 0
}

# Function hrtimer_stress_test
# Parameters:
# 1) Timer test loop for each thread
# 2) The number of timer to open
# return 0 on success otherwise return 1
hrtimer_stress_test(){
	if [ -z "$1" ] || [ -z "$2" ] || [ $1 -eq 0 ] || [ $2 -eq 0 ]; then
		echo "ERROR: failed to get parameter"
		return 1
	fi

	hrtimer_test $1 $2
	if [ $? -ne 0 ]; then
		return 1
	fi

	return 0
}

# Begin script execution here
nominal_test=0
stress_test=0
custom_test=0
hrtimer_loop=0
hrtimer_num=0

while [ $# -gt 0 ]
do
    case $1 in
    -n | --nominal)
    nominal_test=1
    shift 1
    ;;
    -s | --stress)
    stress_test=1
    hrtimer_loop=50000
	hrtimer_num=10
    shift 1
    ;;
    -c | --custom)
    custom_test=1
    hrtimer_loop=$2
	hrtimer_num=$3
    shift 3
    ;;
    -h | --help | *)
    echo "Usage: $0 [-n | --nominal] [-s | --stress] [(-c | --custom) <loop> <timer_number>]"
	echo "	loop: Timer test loop for each thread"
	echo "	timer_number: The number of timer to open"
    exit 1
    ;;
    esac
done

if [ $nominal_test -eq 0 -a $stress_test -eq 0 -a $custom_test -eq 0 ]; then
    nominal_test=1
fi

#init hrtimer
hrtimer_test_init
if [ $? -ne 0 ]; then
	exit 1
fi

result=1
#do nominal test
if [ $nominal_test -eq 1 ]; then
	echo "=== Running Nominal Test ==="
	hrtimer_nominal_test
    if [ $? -eq 0 ]; then
		echo "Nominal Test Passed"
		result=0
    else
		echo "Nominal Test Failed"
		result=1
    fi
fi

#do stress_test test
if [ $stress_test -eq 1 ]; then
	echo "=== Running stress_test Test ==="
	hrtimer_stress_test $hrtimer_loop $hrtimer_num
    if [ $? -eq 0 ]; then
		echo "Stress Test Passed"
		result=0
    else
		echo "Stress Test Failed"
		result=1
    fi
fi

#do custom_test test
if [ $custom_test -eq 1 ]; then
	echo "=== Running custom_test Test ==="
	hrtimer_stress_test $hrtimer_loop $hrtimer_num
    if [ $? -eq 0 ]; then
		echo "Custom Test Passed"
		result=0
    else
		echo "Custom Test Failed"
		result=1
    fi
fi

# remove hrtimer_test_module after test
lsmod | grep "$hrtimer_module_name"
if [ $? -eq 0 ]; then
	rmmod $hrtimer_module_name > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to remove module $hrtimer_test_mod"
		exit 1
	fi
fi

exit $result
