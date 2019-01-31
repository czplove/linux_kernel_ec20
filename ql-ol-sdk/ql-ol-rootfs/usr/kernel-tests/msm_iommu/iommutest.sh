# Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.
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

#  Unit test for IOMMU.
#
# Invoked by run.sh
# This will run nominal and adversarial testcases.

chmod 755 msm_iommutest

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
fi

iommu_test_mod=${modpath}/msm_iommu_test_module.ko
iommu_test_dev=/dev/msm_iommu_test
iommu_test_dev_sys=/sys/class/msm_iommu_test/msm_iommu_test/dev
test_type=n
level=0
force=0
basic=0
repeat=1
iommu_to_test=all

maybe_make_node()
{
	thedev=$1
	themisc=$2
	[[ -e $thedev ]] && return
	type cut >/dev/null && have_cut=yes
	type awk >/dev/null && have_awk=yes
	type sed >/dev/null && have_sed=yes
	type grep >/dev/null && have_grep=yes
	if [[ $have_cut = yes ]]; then
		mknod $thedev c $(cut -d: -f1 $themisc) $(cut -d: -f2 $themisc)
	elif [[ $have_awk = yes ]]; then
		mknod $thedev c $(awk -F: '{print $1}' $themisc) $(awk -F: '{print $2}' $themisc)
	elif [[ $have_sed = yes ]]; then
		mknod $thedev c $(sed 's/\([[:digit:]]\+\):.*/\1/' $themisc) $(sed 's/.*:\([[:digit:]]\+\)/\2/' $themisc)
	elif [[ $have_grep = yes ]]; then
		# we don't have sed, awk, or cut. this is going to get
		# ugly. Use repeated applications of grep's `-o' option to
		# trim things out.
		mknod $thedev c $(grep -Eoh '[[:digit:]]+:' $themisc | grep -Eoh '[[:digit:]]') $(grep -Eoh ':[[:digit:]]+' $themisc | grep -Eoh '[[:digit:]]')
	else
		echo "Can't create $thedev because we don't have cut, sed, awk, grep"
		echo "or a magnetized needle and a steady heand."
		return 1
	fi
	return 0
}

#Parse the args for valid switches
while [ $# -gt 0 ]; do
        case $1 in
	-n | --nominal)
		shift 1
		;;
	-a | --adversarial)
		test_type=a
		shift 1
		;;
	-r | --repeatability)
		test_type=r
		shift 1
                if [ "$1" -eq "$1" ] ; then
                        echo "Specified $1 iterations"
                        repeat=$1
                        shift 1
                else
                        echo "Invalid number of iterations"
                        exit 1
                fi
		;;
	-s | --stress)
		test_type=s
		shift 1
		;;
	-f | --force)
		force=1
		shift 1
	;;
	-b | --basic)
		basic=1
		shift 1
	;;
	-i | --iommu)
		shift 1
                echo "Specified $1 iommu"
		iommu_to_test=$1
		shift 1
	;;
	-v | --verbose)
		level=1
		shift 1
		;;
	-h | --help | *)
		echo "Usage: $0 [-n | --nominal] [-a | --adversarial] [-s | --stress] [-r <num> | --repeatabiliy <num>] [-v] [-b | --basic] [-f | --force] [-i | --iommu] <name of iommu to test>";
		exit 1
		;;
        esac
done

# insert msm_iommu_test_module if needed
if [ ! -e $iommu_test_dev_sys ]; then
	insmod $iommu_test_mod

	if [ $? -ne 0 ]; then
		echo "ERROR: failed to load module $iommu_test_mod"
		exit 1
	fi
fi

# create the iommu test device if it doesn't exist
maybe_make_node $iommu_test_dev $iommu_test_dev_sys || exit 1


#invoke test
./msm_iommutest $test_type $repeat $level $force $basic $iommu_to_test
ret=$?
if [ $ret -ne 0 ];then
	echo "Test Failed"
else
	echo "Test Passed"
fi
rmmod msm_iommu_test_module
exit $ret
