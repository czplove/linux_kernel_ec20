# Copyright (c) 2014, The Linux Foundation. All rights reserved.
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

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
	cp $modpath/modsign_test_module.ko /data/modsign_test_module.ko
	modpath=/data
elif [ -d /kernel-tests/modules/lib/modules/$(uname -r)/extra ]; then
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
elif [ -d /kernel-tests/modules/lib/modules/$(uname -r)-dirty/extra ]; then
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)-dirty/extra
fi

modsign_test_module=${modpath}/modsign_test_module.ko
modsign_test_module_name=modsign_test_module
test_result=1

if [ $# -ne 0 ];then
	while [ $# -gt 0 ]
	do
		case $1 in
			-n | --nominal | -r | --repeatability | -a | --adversarial | -s | --stress)
			shift 1
			;;
			-h | --help | *)
			echo "Usage: $0 [-n | --nominal] [-r | --repeatability] [-a | --adversarial] [-s | --stress]"
			exit 1
			;;
		esac
	done
fi

modsign_test() {
	if [ ! -d $modpath ]; then
		echo "ERROR: Couldn't find a path to the kernel module."
		test_result=1
		return;
	fi

	# remove modsign_test_module before test
	lsmod | grep "$modsign_test_module_name"
	if [ $? -eq 0 ]; then
		rmmod $modsign_test_module > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			echo "ERROR: failed to remove module $modsign_test_module_name"
			test_result=1
			return;
		fi
	fi

	insmod $modsign_test_module
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to load module $modsign_test_module_name"
		test_result=1
		return;
	fi

	rmmod $modsign_test_module > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to remove module $modsign_test_module_name"
		test_result=1
		return;
	fi

	sed -i '1s/^/z/' $modsign_test_module

	insmod $modsign_test_module
	if [ $? -eq 0 ]; then
		echo "ERROR: $modsign_test_module_name signed module got loaded even after being modified."
		test_result=1
		return;
	fi

	sed -i '1s/.//' $modsign_test_module

	insmod $modsign_test_module
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to load module $modsign_test_module_name"
		test_result=1
		return;
	fi

	# remove modsign_test_module after test
	rmmod $modsign_test_module > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "ERROR: failed to remove module $modsign_test_module_name"
		test_result=1
		return;
	fi

	test_result=0
	return;
}

modsign_test
if [ $? -ne 0 ]; then
	exit 1
fi

if [ "$test_result" -eq "0" ]; then
	echo "Test passed"
	exit 0
else
	echo "Test failed"
	exit 1
fi
