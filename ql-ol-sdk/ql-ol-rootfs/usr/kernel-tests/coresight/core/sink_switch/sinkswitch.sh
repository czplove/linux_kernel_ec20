#!/bin/sh

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

etfsink="/sys/bus/coresight/devices/coresight-tmc-etf/curr_sink"
etrsink="/sys/bus/coresight/devices/coresight-tmc-etr/curr_sink"
tpiusink="/sys/bus/coresight/devices/coresight-tpiu/curr_sink"
tpiumode="/sys/bus/coresight/devices/coresight-tpiu/out_mode"

echo "-----Coresight Sink Switching Test-----"
echo "---------------------------------------"
source=`echo "$*" | sed -n 's/.*--source \(\w*\).*/\1/p'`
source=`echo $source | tr '[A-Z]' '[a-z]'`
#enable ETM and STM source
source "$(dirname $0)/../cs_common.sh"
if [[ -z "$source" ]]
then
        source="all"
fi
if [[ $source != "stm" ]]
then
        etm_enable_all_cores
fi
if [[ $source != "etm" ]]
then
        stm_enable
fi
echo 1 > $etrsink
read etrstatus < $etrsink
if [ $etrstatus = 1 ]
then
        echo "PASS: Changed current sink to ETR"
else
        echo "FAIL: Sink Switching to ETR failed ****"
fi
echo "sdc" > $tpiumode
echo 1 > $tpiusink
read tpiustatus < $tpiusink
if [ $tpiustatus = 1 ]
then
        echo "PASS: Changed current sink to TPIU"
else
        echo "FAIL: Sink Switching to TPIU failed ****"
fi
echo 1 > $etfsink
read etfstatus < $etfsink
if [ $etfstatus = 1 ]
then
        echo "PASS: Changed current sink to ETF"
else
        echo "FAIL: Sink Switching to ETF failed ****"
fi
#test if trace sources are still enabled
if [[ $source != "stm" ]]
then
        etm_test_if_enabled
        retval=$?
        if [ $retval -eq 0 ]
        then
                echo "PASS: All ETM devices enabled after sink switch"
        fi
fi
if [[ $source != "etm" ]]
then
        stm_test_if_enabled
        retval=$?
        if [ $retval -eq 0 ]
        then
                echo "PASS: STM Enabled after sink switch"
        fi
fi
if [[ $source != "stm" ]]
then
        etm_disable_all_cores
fi
if [[ $source != "etm" ]]
then
        stm_disable
fi
echo "----- Coresight Sink Switching Test Complete-----"
echo "-------------------------------------------------"
echo ""
