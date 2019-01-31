#!/bin/sh --

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

set -e

if [ $# -eq 0 ]
then
	cd `dirname $0` && exec ./stm_test.sh 10 10 0.001
	exit 0
fi

#Parse the args for valid switches
while [ $# -gt 0 ]; do
        case $1 in
	-n | --nominal)
		cd `dirname $0` && exec ./stm_test.sh 10 10 0.001
		exit 0
		;;
	-a | --adversarial)
		cd `dirname $0` && exec ./stm_test.sh 50 10 0.001
		exit 0
		;;
	-r | --repeatability)
		echo $1
                if [[ $# -eq 2 && "$2" -gt 0 ]]; then
                        echo "Specified $2 iterations"
			cnt=0
			while [ $cnt -le $2 ]
			do
				cd `dirname $0` && ./stm_test.sh 10 10 0.001
				cnt=$(($cnt+1))
			done
			exit 0
                else
                        echo "Invalid number of iterations"
                        exit 1
                fi
		;;
	-s | --stress)
		cd `dirname $0` && exec ./stm_test.sh 100 10 0.001
		exit 0
		;;
	-v | --verbose)
		cd `dirname $0` && exec ./stm_test.sh 10 10 0.001
		exit 0
		;;
	-h | --help | *)
		echo "Usage: ./run.sh -[n][a][r][s]"
		exit 1
		;;
        esac
done
