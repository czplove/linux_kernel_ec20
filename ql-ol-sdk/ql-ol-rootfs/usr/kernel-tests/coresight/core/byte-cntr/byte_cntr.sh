#! /bin/sh

# Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
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


echo "-----Coresight Byte Counter Test Starting-----"
echo "--------------------------------------------"
source "$(dirname $0)/../cs_common.sh"
if [[ -z "$source" ]]
then
        source="all"
fi
data_size=262144
stm_hwevent=$stmpath"/hwevent_enable"
if [ -e "/dev/coresight-tmc-etr-stream" ]
then
	stm_disable
	if [[ $source != "stm" ]]
	then
		etm_disable_all_cores
	fi
	# Configure byte counter to generate interrupts after each 1KB of data
	echo 0x80 > $tmcetrpath"/byte_cntr_value"
	# Make etr current trace sink
	echo mem > $tmcetrpath"/out_mode"
	echo 1 > $tmcetrpath"/curr_sink"
	stm_enable
	# Disable hwevents when stm is enabled
	echo 0 > $stm_hwevent
	cat "/dev/coresight-tmc-etr-stream" > "/data/byte-cntr-etr.bin" &
	# Make sure the backgroud process has a chance to run
	sleep 10
	echo "Start trace" > /dev/coresight-stm
	# Generate at least 2MB of data
	for i in $(seq $data_size)
	do
		echo "12345678" > /dev/coresight-stm
	done
	echo "End trace" > /dev/coresight-stm
	size=$(stat -c  %s "/data/byte-cntr-etr.bin")
	size=$(($size / 8))
	if [ $size -ge $data_size ]
	then
		echo "PASS: Byte Counter test"
	else
		echo "FAIL: Byte Counter test"
	fi
	stm_disable
else
        echo "FAIL: Byte Counter test-feature not present"
fi
echo "-----Coresight Byte Counter Test Complete-----"
echo "--------------------------------------------"
echo ""
