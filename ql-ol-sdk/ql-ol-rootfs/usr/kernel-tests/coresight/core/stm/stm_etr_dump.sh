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

echo "-----Coresight ETR Dump Test for STM Starting-----"
echo "--------------------------------------------------"
source "$(dirname $0)/../cs_common.sh"
stm_port=$stmpath"/port_enable"
stm_hwevent=$stmpath"/hwevent_enable"
if [ ! -d $debugfs ]
then
        mkdir $debufgs
fi
mount -t debugfs nodev $debugfs 2>/dev/null
echo 0 > $debugfs"/tracing/events/enable"
#make etr current trace sink
echo 1 > $tmcetrpath"/curr_sink"
stm_enable
#disable hwevents when stm is enabled
echo 0 > $stm_hwevent
echo "stm-dump" > "/dev/coresight-stm"
stm_disable
if [ ! -d "/data/coresight-test" ]
then
        mkdir -p "/data/coresight-test"
fi
cat "/dev/coresight-tmc-etr" > "/data/coresight-test/stm_etr.bin"
chmod a+x "/data/coresight-test/stm_etr.bin"
hexdump -ve '1/1 "%.2X"' "/data/coresight-test/stm_etr.bin" > "/data/coresight-test/stm_etr.txt"
brk=0
skip=0
size=0
while [ $brk -eq 0 ]
do
        dd if="/data/coresight-test/stm_etr.txt" bs=1 skip=$skip count=16 of="/data/coresight-test/stm_etr.out" 2> /dev/null
        if grep '0\{16\}' "/data/coresight-test/stm_etr.out" > /dev/null
        then
                brk=1
        else
                size=$(( size + 16 ))
        fi
        skip=$(( skip + 16 ))
done
if [ $size -gt 64 ]
then
        echo "PASS: STM ETR dump test"
else
        echo "FAIL: STM ETR dump test"
fi
rm -r "/data/coresight-test"
umount  "/sys/kernel/debug" 2>/dev/null
echo "-----Coresight ETR Dump Test for STM Complete-----"
echo "--------------------------------------------------"
echo ""
