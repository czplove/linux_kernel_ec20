#!/bin/sh

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

online_offline_cores() {
cores=`grep -c "processor" $cpuinfo`
count=1
loop=10
stop mpdecision
#off-on each core in sequence. Repeat 10 times
for i in $(seq 1 $loop)
do
        while [ $count -lt $cores ]
        do
                echo 0 > $cpupath$count"/online"
                sleep 1
                echo 1 > $cpupath$count"/online"
                count=$(( count + 1 ))
        done
done
#off-on multiple cores at a time. Repeat 10 times
loop=10
count=1
for i in $(seq 1 $loop)
do
        while [ $count -lt $cores ]
        do
                echo 0 > $cpupath$count"/online"
                count=$(( count + 1 ))
        done
        sleep 1
        count=1
        while [ $count -lt $cores ]
        do
                echo 1 > $cpupath$count"/online"
                count=$(( count + 1 ))
        done
done
start mpdecision
}

echo "-----Coresight Trace Driver Adversarial Test Starting-----"
echo "----------------------------------------------------------"
source "$(dirname $0)/cs_common.sh"
etm_enable_all_cores
stm_enable
if [ ! -d $debugfs ]
then
      mkdir -p $debugfs
fi
mount -t debugfs nodev $debugfs 2>"/dev/null"
echo 1 > $debugfs"/tracing/tracing_on"
echo 1 > $debugfs"/tracing/events/enable"
echo "*:*" > $debugfs"/tracing/set_event"
online_offline_cores
etm_disable_all_cores
stm_disable
umount $debugfs 2>"/dev/null"
echo "PASS: Online-Offline all cores with trace drivers enabled"
echo "-----Coresight Trace Driver Adversarial Test Complete-----"
echo "----------------------------------------------------------"
echo ""
