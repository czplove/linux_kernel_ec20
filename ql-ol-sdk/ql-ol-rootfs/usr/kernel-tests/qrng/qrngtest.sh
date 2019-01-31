# Copyright (c) 2016, The Linux Foundation. All rights reserved.
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

#Get the command line arguments
cmd_line_args=$@
args_count=$#

#Change the permissions for qrngtest
chmod 755 qrngtest

sysrng_dev=/dev/random

if [ ! -e $sysrng_dev ]; then
	echo "ERROR: random device is not present"
	exit 1
fi

#Parse the arguments list
#only -n,-a,-r,-s,-d,-c,-v,-h are allowed for automation tests
while [ $# -gt 0 ]; do
        case $1 in
	-n)
		shift 1
		;;
	-a)
		shift 1
		;;
	-r)
		shift 1
		;;
	-s)
		shift 1
		;;
	-d)
		if [ ! $2 ]; then
			echo "Please provide the device name"
			exit 1
		fi
		shift 2
		;;
	-c)
		shift 1
		;;
	-v)
		shift 1
		;;
	-h | *)
		shift 1
		;;
	esac
done

#Execute the tests
#If there are 0 arguments the default test case to be run 100 times
if [ $args_count -ne 0 ]; then
	./qrngtest $cmd_line_args
else
	./qrngtest
fi

retval=$?

#Check for pass or fail status
if [ $retval -ne 0 ];then
	return_value=1;
else
	return_value=0;
fi

#return with exit value 0(PASS)/1(FAIL)
exit $return_value
