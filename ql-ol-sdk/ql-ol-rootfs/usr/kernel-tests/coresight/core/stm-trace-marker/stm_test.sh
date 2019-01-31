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

runs=49
sets=10
delay=0.001

if [ $# -gt 4 ]
then
	echo "Usage: ./stm-test.sh [runs] [run-size] [delay] [usb|mem]"
	echo "Delay is specified in seconds."
	echo "To specify smaller delays use fractions. For 1ms, use 0.001"
	exit
else
	if [ $# -eq 1 ]
	then
		runs=$(($1-1))
		echo 1 > /sys/bus/coresight/devices/coresight-tmc-etf/curr_sink
	elif [ $# -eq 2 ]
	then
		runs=$(($1-1))
		sets=$2
		echo 1 > /sys/bus/coresight/devices/coresight-tmc-etf/curr_sink
	elif [ $# -eq 3 ]
	then
		runs=$(($1-1))
		sets=$2
		delay=$3
		echo 1 > /sys/bus/coresight/devices/coresight-tmc-etf/curr_sink
	elif [ $# -eq 4 ]
	then
		runs=$(($1-1))
		sets=$2
		delay=$3
		if [ $4 = "usb" ] || [ $4 = "mem" ]
		then
			echo $4 > /sys/bus/coresight/devices/coresight-tmc-etr/out_mode
			echo 1 > /sys/bus/coresight/devices/coresight-tmc-etr/curr_sink
		else
			echo 1 > /sys/bus/coresight/devices/coresight-tmc-etf/curr_sink
		fi
	fi
fi

echo "STM TEST"

# Mount debugfs
mount -t debugfs nodev /sys/kernel/debug

# create output directory
if [ ! -d "/data" ]
then
	mkdir /data
fi
if [ ! -d "/data/coresight" ]
then
	mkdir /data/coresight
fi

# Enable stm
echo 1 > /sys/bus/coresight/devices/coresight-stm/enable
# Disable hw events
echo 0 > /sys/bus/coresight/devices/coresight-stm/hwevent_enable

# Insert an text marker to indicate start of current test
echo "Starting trace" > /sys/kernel/debug/tracing/trace_marker

# 'X' Sets of 'Y' events with delay
for i in $(seq 0 $runs)
do
	echo "Run $(($i+1)): Generating $sets events and sleeping for $delay sec"
	for j in $(seq 1 $sets)
	do
		echo "Event $(($i*$sets+$j))" > /sys/kernel/debug/tracing/trace_marker
	done
	sleep $delay
done

# Again insert text marker to indicate end of test
echo "Stopping trace" > /sys/kernel/debug/tracing/trace_marker

# Disable stm
echo 0 > /sys/bus/coresight/devices/coresight-stm/enable

# dump trace from etf
if [ $# -eq 4 ]
then
	if [ $4 == "mem" ]
	then
		cat /dev/coresight-tmc-etr > /data/coresight/stm_trace.bin
	elif [ $4 != "usb" ]
	then
		cat /dev/coresight-tmc-etf > /data/coresight/stm_trace.bin
	fi
else
	cat /dev/coresight-tmc-etf > /data/coresight/stm_trace.bin
fi

cat /sys/kernel/debug/tracing/trace > /data/coresight/trace.txt

# Get format file
cat /sys/kernel/debug/tracing/events/*/*/format > /data/coresight/formats.txt

echo "TEST COMPLETE"

exit 0
