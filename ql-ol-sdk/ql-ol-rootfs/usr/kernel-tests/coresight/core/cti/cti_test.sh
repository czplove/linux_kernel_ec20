#! /bin/sh --

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

# Script to test the QDSS CTI driver.
echo "-----Coresight cti Test Starting-----"
echo "-------------------------------------"
source "$(dirname $0)/../cs_common.sh"

CTI_MAX_TRIGGERS=8
CTI_MAX_CHANNELS=4
CTI_MAX_NUMBERS=`ls /sys/bus/coresight/devices | grep 'coresight-cti[0-8]'| wc -l`
if [ $CTI_MAX_NUMBERS -lt 0 ]; then
    echo "FAIL: No CTI device found"
    exit 1
fi

cti_number=$(($CTI_MAX_NUMBERS-1))
cti_max_trig_index=$(($CTI_MAX_TRIGGERS-1))
cti_max_ch_index=$(($CTI_MAX_CHANNELS-1))
trig_rang=$( seq 0 $cti_max_trig_index)
channel_rang=$( seq 0 $cti_max_ch_index)
cti_index_rang=$( seq 0 $cti_number)

#failure count
fail_cnt=0
for index in $cti_index_rang
do
    for trig in $trig_rang
    do
        for channel in $channel_rang
        do
            echo $trig $channel > $ctipath$index/map_trigin
            trigin=`cat $ctipath$index/show_trigin | cut -b 4`
            channelin=`cat $ctipath$index/show_trigin | cut -b 8`
            if [ "$trig" -eq "$trigin" ] && [ "$channel" -eq "$channelin" ]; then
                echo $trig $channel > $ctipath$index/unmap_trigin
                trigin=`cat $ctipath$index/show_trigin | cut -b 4`
                channelin=`cat $ctipath$index/show_trigin | cut -b 8`
                if [ "$trigin" != "" ] || [ "$channelin" != "" ]; then
                    echo "Failed to unmap coresight-cti$index trigin $trig to channel $channel"
                    fail_cnt=$(($fail_cnt+1))
                    echo 1 > $ctipath$index/reset
                fi
            else
                echo "Failed to map coresight-cti$index trigin $trig to channel $channel"
                fail_cnt=$(($fail_cnt+1))
                echo 1 > $ctipath$index/reset
            fi

            echo $trig $channel > $ctipath$index/map_trigout
            trigout=`cat $ctipath$index/show_trigout | cut -b 4`
            channelout=`cat $ctipath$index/show_trigout | cut -b 8`
            if [ "$trig" -eq "$trigout" ] && [ "$channel" -eq "$channelout" ]; then
                echo $trig $channel > $ctipath$index/unmap_trigout
                trigout=`cat $ctipath$index/show_trigout | cut -b 4`
                channelout=`cat $ctipath$index/show_trigout | cut -b 8`
                if [ "$trigout" != "" ] || [ "$channelout" != "" ]; then
                    echo "Failed to unmap coresight-cti$index trigout $trig to channel $channel"
                    fail_cnt=$(($fail_cnt+1))
                    echo 1 > $ctipath$index/reset
                fi
            else
                echo "Fail to map coresight-cti$index trigout $trig to channel $channel"
                fail_cnt=$(($fail_cnt+1))
                echo 1 > $ctipath$index/reset
            fi
        done
    done
    index=$(($index+1))
done

if [ "$fail_cnt" -eq "0" ]; then
    echo "CTI map/unmap Test PASS "
else
    echo "CTI map/unamp Test FAIL "
fi
