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


cpupath="/sys/devices/system/cpu/cpu"
cpuinfo="/proc/cpuinfo"
csrpath="/sys/bus/coresight/devices/coresight-csr"
debugfs="/sys/kernel/debug"
etmpath="/sys/bus/coresight/devices/coresight-etm"
f0path="/sys/bus/coresight/devices/coresight-funnel-in0"
f1path="/sys/bus/coresight/devices/coresight-funnel-in1"
fmergpath="/sys/bus/coresight/devices/coresight-funnel-merg"
fkpsspath="/sys/bus/coresight/devices/coresight-funnel-kpss"
fmmsspath="/sys/bus/coresight/devices/coresight-funnel-mmss"
replicatorpath="/sys/bus/coresight/devices/coresight-replicator"
stmpath="/sys/bus/coresight/devices/coresight-stm"
tmcetfpath="/sys/bus/coresight/devices/coresight-tmc-etf"
tmcetrpath="/sys/bus/coresight/devices/coresight-tmc-etr"
tpiupath="/sys/bus/coresight/devices/coresight-tpiu"
ctipath="/sys/bus/coresight/devices/coresight-cti"


#===============================================================
#function to enable etm for all cores
#returns 0 if etm for all cores are present and could be written to, 1 otherwise
#===============================================================
etm_enable_all_cores() {
count=0
retval=0
cores=`grep -c "processor" $cpuinfo`
if [ $cores -gt 1 ]
then
	stop mpdecision
fi
while [ $count -lt $cores ]
do
	if [ $count -gt 0 ]
	then
		cpupath_enable=$cpupath$count"/online"
		echo 1 > $cpupath_enable
	fi
        etmpath_enable=$etmpath$count"/enable"
        if [ -f $etmpath_enable ]
        then
                echo 1 > $etmpath_enable
        else
                echo "FAIL: No coresight-etm$count driver present ****"
                retval=1
        fi
        count=$(( count + 1 ))
done
if [ $cores -gt 1 ]
then
	start mpdecision
fi
return $retval
}

#=============================================================
#function to test if etm of all cores are enabled
#returns 0 if all cores are enabled, 1 otherwise
#=============================================================
etm_test_if_enabled() {
count=0
retval=0
cores=`grep -c "processor" $cpuinfo`
while [ $count -lt $cores ]
do
        etmpath_enable=$etmpath$count"/enable"
        if [ -f $etmpath_enable ]
        then
                read value < $etmpath_enable
                if [ $value != "1" ]
                then
                        echo "FAIL: coresight-etm$count device not enabled ****"
                        retval=1
                fi
        else
                retval=1
        fi
        count=$(( count + 1 ))
done
return $retval
}

#===============================================================
#function to disable etm for all cores
#returns 0 if etm for all cores are present and could be written to, 1 otherwise
#===============================================================
etm_disable_all_cores() {
count=0
retval=0
cores=`grep -c "processor" $cpuinfo`
if [ $cores -gt 1 ]
then
	stop mpdecision
fi
while [ $count -lt $cores ]
do
	if [ $count -gt 0 ]
	then
		cpupath_enable=$cpupath$count"/online"
		echo 1 > $cpupath_enable
	fi
        etmpath_enable=$etmpath$count"/enable"
        if [ -f $etmpath_enable ]
        then
                echo 0 > $etmpath_enable
        else
                echo "FAIL: No coresight-etm$count driver present ****"
                retval=1
        fi
        count=$(( count + 1 ))
done
if [ $cores -gt 1 ]
then
	start mpdecision
fi
return $retval
}

#=============================================================
#function to test if etm of all cores are disabled
#returns 0 if all cores are disabled, 1 otherwise
#=============================================================
etm_test_if_disabled() {
count=0
retval=0
cores=`grep -c "processor" $cpuinfo`
while [ $count -lt $cores ]
do
        etmpath_enable=$etmpath$count"/enable"
        if [ -f $etmpath_enable ]
        then
                read value < $etmpath_enable
                if [ $value != "0" ]
                then
                        echo "FAIL: coresight-etm$count device not disabled ****"
                        retval=1
                fi
        else
                retval=1
        fi
        count=$(( count + 1 ))
done
return $retval
}


#==============================================================
#function to enable stm
#returns 0 if device is written to, 1 otherwise
#==============================================================
stm_enable() {
stmpath_enable=$stmpath"/enable"
if [ -f $stmpath_enable ]
then
        echo 1 > $stmpath_enable
        retval=0
else
        echo "FAIL: No coresight-stm driver present ****"
        retval=1
fi
return $retval
}

#==============================================================
#function to test if stm is enabled
#returns 0 on success, 1 otherwise
#==============================================================
stm_test_if_enabled() {
stmpath_enable=$stmpath"/enable"
if [ -f $stmpath_enable ]
then
        read value < $stmpath_enable
        if [ $value = "1" ]
        then
                retval=0
        else
                echo "FAIL: coresight-stm device not enabled ****"
                retval=1
        fi
else
        retval=1
fi
return $retval
}

#==============================================================
#function to disable stm
#returns 0 if device could be written to, 1 otherwise
#==============================================================
stm_disable() {
stmpath_enable=$stmpath"/enable"
if [ -f $stmpath_enable ]
then
        echo 0 > $stmpath_enable
        retval=0
else
        echo "FAIL: No coresight-stm driver present ****"
        retval=1
fi
return $retval
}

#===============================================================
#function to test if stm is disabled
#returns 0 on success, 1 otherwise
#===============================================================
stm_test_if_disabled() {
stmpath_enable=$stmpath"/enable"
if [ -f $stmpath_enable ]
then
        read value < $stmpath_enable
        if [ $value = "0" ]
        then
                retval=0
        else
                echo "FAIL: coresight-stm device not disabled ****"
                retval=1
        fi
else
        retval=1
fi
return $retval
}


