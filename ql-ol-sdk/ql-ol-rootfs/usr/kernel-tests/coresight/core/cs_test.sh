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

# Invoked by run.sh

directory=$(cd `dirname $0` && pwd)

#Requirement: Before running test, ensure STM/ETM is initially disabled
cs_test_setup(){
source "$(dirname $0)/cs_common.sh"
retval=0
if [ $stm -eq 1 ]
then
        stm_disable
        retval=$?
fi
if [ $retval -eq 0 ] && [ $etm -eq 1 ]
then
        etm_disable_all_cores
        retval=$?
fi
if [ $retval -ne 0 ]
then
        echo "Requirement for running test:"
        echo "ETM/STM cannot be disabled. Exiting tests"
        exit 1
fi
}

cs_nominal(){
cd $directory"/platform" && sh platform.sh "--source $source"
if [ $etm -eq 1 ]
then
        cd $directory"/etm" && sh etm_enable.sh
        cd $directory"/etm" && sh etm_disable.sh
fi
if [ $stm -eq 1 ]
then
        cd $directory"/stm" && sh stm_enable.sh
        cd $directory"/stm" && sh stm_disable.sh
        cd $directory"/stm" && sh stm_etf_dump.sh
        cd $directory"/stm" && sh stm_etr_dump.sh
        cd $directory"/byte-cntr" && sh byte_cntr.sh "--source $source"
fi
if [ $stm -eq 1 ] && [ $etm -eq 1 ]
then
        cd $directory"/mult_trace" && sh mult_source_enable.sh
        cd $directory"/mult_trace" && sh mult_source_disable.sh
fi
cd $directory"/sink_switch" && sh sinkswitch.sh "--source $source"
cd $directory"/sink_switch" && sh etr_modes.sh "--source $source"
cd $directory"/profile" && sh cs_profile.sh "--source $source"
cd $directory"/cti" && sh cti_test.sh
}

cs_adversary(){
if [ $stm -eq 1 ] && [ $etm -eq 1 ]
then
        cd $directory && sh cs_adversary.sh
else
        echo "Both STM and ETM required as trace source."
fi
}

cs_repeatability(){
if [ $# -eq 1 ]
then
        run=$1
else
        run=10
fi
if [ $etm -eq 1 ]
then
        echo "Coresight ETM enable/disable repeat test started for $run
              iterations"
        for i in $(seq 1 $run)
        do
                cd $directory"/etm" && sh etm_enable.sh
                cd $directory"/etm" && sh etm_disable.sh
        done
fi
if [ $stm -eq 1 ]
then
        echo "Coresight STM enable/disable repeat test started for $run
              iterations"
        for i in $(seq 1 $run)
        do
                cd $directory"/stm" && sh stm_enable.sh
                cd $directory"/stm" && sh stm_disable.sh
        done
        echo "Coresight STM ETF dump test started for $run iterations"
        for i in  $(seq 1 $run)
        do
                cd $directory"/stm" && sh stm_etf_dump.sh
        done
        echo "Coresight STM ETR dump test started for $run iterations"
        for i in  $(seq 1 $run)
        do
                cd $directory"/stm" && sh stm_etr_dump.sh
        done
fi
if [ $stm -eq 1 ] && [ $etm -eq 1 ]
then
        echo "Coresight multi trace enable/disable repeat test started for
              $run iterations"
        for i in $(seq 1 $run)
        do
                cd $directory"/mult_trace" && sh mult_source_enable.sh
                cd $directory"/mult_trace" && sh mult_source_disable.sh
        done
fi
echo "Coresight sink switching repeat test started for $run iterations"
for i in $(seq 1 $run)
do
        cd $directory"/sink_switch" && sh sinkswitch.sh "--source $source"
done
echo "Coresight ETR modes change started for $run iterations"
for i in $(seq 1 $run)
do
        cd $directory"/sink_switch" && sh etr_modes.sh "--source $source"
done
echo "Coresight cti test started"
for i in $(seq 1 $run)
do
        cd $directory"/cti" && sh cti_test.sh
done

if [ $stm -eq 1 ] && [ $etm -eq 1 ]
then
        echo "CoreSight adversarial repeat test started for $run iterations"
        for i in $(seq 1 $run)
        do
                cd $directory && sh cs_adversary.sh
        done
fi
}

cs_stress(){
run=100
echo "Coresight stress test started"
for i in $(seq 1 $run)
do
        cs_adversary
done
}

source=`echo "$*" | sed -n 's/.*--source \(\w*\).*/\1/p'`
source=`echo $source | tr '[A-Z]' '[a-z]'`
if [[ "$source" ]]
then
        if [[ $source == "stm" ]]
        then
                echo "All STM tests will be run"
                stm=1
                etm=0
        elif [[ $source == "etm" ]]
        then
                echo "All ETM tests will be run"
                stm=0
                etm=1
        fi
else
        stm=1
        etm=1
fi
if [[ $# -le 2 && -n "$source" || $# -eq 0 ]]
then
        cs_test_setup
        cs_nominal
        exit 0
else
        while [ $# -gt 0 ]
        do
                case $1 in
                -n | --nominal)
                        echo "Coresight nominal test started"
                        cs_test_setup
                        cs_nominal
                        shift 1
                        ;;
                -a | --adversarial)
                        echo "Coresight adversarial test started"
                        cs_test_setup
                        cs_adversary
                        shift 1
                        ;;
                -r | --repeatability)
                        echo "Coresight repeatability test started"
                        shift 1
                        if [ "$1" -gt 0 ]
                        then
                                cs_test_setup
                                cs_repeatability $1
                                shift 1
                        else
                                echo "Invalid number of iterations, doing 10 iterations by default"
                                cs_test_setup
                                cs_repeatability 10
                        fi
                        ;;
                -s | --stress)
                        echo "Coresight stress test started"
                        cs_test_setup
                        cs_repeatability 100
                        shift 1
                        ;;
                --source)
                        shift 1
                        shift 1
                        ;;
                -h | --help | *)
                        echo "Usage: $0 [-n | --nominal] [-a | --adversarial ] \\ "
                        echo "       [-r | --repeatability] [iterations] [-s | --stress ] \\"
                        echo "       [--source] [trace_source]"
                        echo "Runs the coresight driver tests. If no options are provided "
                        echo "then nominal tests are run. If no iterations are provided for "
                        echo "repeatability tests, 10 is the default iteration. By default "
                        echo "both ETM and STM are enabled"
                        exit 1
                        ;;
                esac
        done
exit 0
fi

