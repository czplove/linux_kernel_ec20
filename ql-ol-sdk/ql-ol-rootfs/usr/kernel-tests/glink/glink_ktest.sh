# Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
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
[ -d /system/lib/modules ] && modpath=/system/lib/modules
[ -d /kernel-tests/modules/lib/modules/$(uname -r 2>/dev/null)/extra ] && modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
[ -d /usr/kernel-tests/glink ] && modpath=/usr/kernel-tests/glink

verbose=0
TEST_DIR="/d/glink/test"

if [[ -z "$modpath" ]]; then
	echo "Couldn't find a path to the kernel module. Bailing."
	echo "Test failed"
	exit 1
fi

glink_test_mod=${modpath}/glink_test.ko

# load_sub() - loads the testing DLKM
load_sub() {
	insmod $glink_test_mod

	if [ $? -ne 0 ]; then
		echo "ERROR: Failed to load module $glink_test_mod"
		echo "Test failed"
		exit
	fi
}

# unload_sub() - unloads the testing DLKM
unload_sub() {
	rmmod $glink_test_mod
}

# echov() - Echo if verbosity flag is set
# $1 - what to echo
echov() {
	if [ $verbose -ne 0 ]; then
		echo "$1"
	fi
}


# Run an individual test
# $1 - test name (fully-qualified path + name)
run_test() {
	local output=''

	echov "Running test $1"
	output=$(cat $1)

	if [[ $outout == *"FAIL"* || $output == *"Fail"* ]]; then
		echo
		echo "$output"
		echo
		rc=1
	else
		# test case passed, only print output in verbose mode
		if [ $verbose -eq 0 ]; then
			echo -n "."
		else
			echo "$output"
			echo ""
		fi
	fi
}

# run_all() - Run all unit tests of specified level
# $1 - level (0, 1, 2, or ? for all)
run_all() {
	# pass in test level (0, 1, 2, or '.' for all) into $1
	for xprt in $(ls $TEST_DIR); do
		for test_name in $(ls $TEST_DIR/$xprt/ut$1* 2>/dev/null); do
			run_test "$test_name"
		done
	done
}

load_sub

rc=0
for param in $*
do
	case $param in
		-h | --help)
			echo "Usage: $0 [-n | --nominal] [-a | --adversarial] [-r | --repeatability] [-s | --stress] [-v | --verbose]"
			break
			;;
		-v | --verbose)
			verbose=1
			;;
		-a | --adversarial)
			run_all "?"
			;;
		-n | --nominal)
			run_all "0"
			;;
		-r | --repeatability)
			n=0
			while [ $n -lt 10]; do
				run_all "?"
				n=$((n+1))
			done
			;;
		-s | --stress)
			run_all "?"
			;;
		*)
			echo "Invalid parameter"
			rc=1
			break
			;;
	esac
done

unload_sub

if [ $rc -eq 0 ]; then
	echo "\nTest passed"
else
	echo "\nTest failed"
fi

exit $rc
