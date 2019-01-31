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

count=0
source=`echo "$*" | sed -n 's/.*--source \(\w*\).*/\1/p'`
source=`echo $source | tr '[A-Z]' '[a-z]'`
source "$(dirname $0)/../cs_common.sh"
if [[ -z "$source" ]]
then
        source="all"
fi
echo "-----Coresight Platform Test Starting-----"
echo "------------------------------------------"
if [ -d $csrpath ]
then
       echo "PASS: coresight-csr driver initialized"
else
       echo "FAIL: coresight-csr driver probe failed ****"
fi
if [[ $source == "etm" || $source == "all" ]]
then
	cores=`grep -c "processor" $cpuinfo`
	while [ $count -lt $cores ]
	do
		if [ -d $etmpath$count ]
		then
			echo "PASS: coresight-etm$count driver initialized"
		else
			echo "FAIL: coresight-etm$count driver probe failed ****"
		fi
		count=$(( count + 1 ))
	done
fi
if [ -d $f0path ]
then
        echo "PASS: coresight-funnel-in0 driver initialized"
else
        echo "FAIL: coresight-funnel-in0 driver probe failed ****"
fi
if [ -d $f1path ]
then
        echo "PASS: coresight-funnel-in1 driver initialized"
else
        echo "FAIL: coresight-funnel-in1 driver probe failed ****"
fi
if [ -d $fmergpath ]
then
        echo "PASS: coresight-funnel-merg driver initialized"
else
        echo "FAIL: coresight-funnel-merg driver probe failed ****"
fi
if [ -d $replicatorpath ]
then
        echo "PASS: coresight-replicator driver initialized"
else
        echo "FAIL: coresight-replicator driver probe failed ****"
fi
if [[ $source == "stm" || $source == "all" ]]
then
	if [ -d $stmpath ]
	then
		echo "PASS: coresight-stm driver initialized"
	else
		echo "FAIL: coresight-stm driver probe failed ****"
	fi
fi
if [ -d $tmcetfpath ] && [ -d $tmcetrpath ]
then
        echo "PASS: coresight-tmc driver initialized"
else
        echo "FAIL: coresight-tmc driver probe failed ****"
fi
if [ -d $tpiupath ]
then
        echo "PASS: coresight-tpu driver initialized"
else
        echo "FAIL: coresight-tpu driver probe failed ****"
fi
echo "-----Coresight Platform Test Complete-----"
echo "------------------------------------------"
echo ""
