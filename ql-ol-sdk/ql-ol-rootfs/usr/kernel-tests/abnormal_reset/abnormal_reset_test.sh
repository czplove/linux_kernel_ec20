#! /bin/sh --

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

ABNORMAL_RESET_DIR=`dirname $0`
TEST_ENV_SETUP=$ABNORMAL_RESET_DIR/../test_env_setup.sh
if [ -f $TEST_ENV_SETUP ]; then
	. $TEST_ENV_SETUP 2>/dev/null
fi

SPMI_PON_PATH="$TEST_DEBUG_FS/spmi/spmi-0"

if [ -d /system/lib/modules/ ]; then
	modpath=/system/lib/modules
else
	modpath=/kernel-tests/modules/lib/modules/$(uname -r)/extra
fi

tsensor_reset_mod=${modpath}/tsensor_reset_mod.ko

case "$TARGET_TYPE" in
	"8916" | "8939" | "8909" | "8952" | "8976")
		SPMI_PON_BASE=0x800;
		SID=0x0;
	;;
	* )
		echo "Unsupport target $TARGET_TYPE"
		exit 1
	;;
esac

SID=$((SID<<16))
PON_SW_RESET_S2_CTL=$(($SID + $SPMI_PON_BASE + 0x62))
PON_SW_RESET_S2_CTL2=$(($SID + $SPMI_PON_BASE + 0x63))
PON_SW_RESET_GO=$(($SID + $SPMI_PON_BASE + 0x64))

software_reset_sequence()
{
	echo "Kernel test: starting software reset sequence\n"
	echo $PON_SW_RESET_S2_CTL > $SPMI_PON_PATH/address
	echo 0x1 0x80 > $SPMI_PON_PATH/data
	echo $PON_SW_RESET_GO > $SPMI_PON_PATH/address
	echo 0xA5 > $SPMI_PON_PATH/data
	sleep 4
	echo "Kernel test: Software reset failed\n"
}

PON_PMIC_WD_RESET_S1_TIMER=$(($SID + $SPMI_PON_BASE + 0x54))
PON_PMIC_WD_RESET_S2_TIMER=$(($SID + $SPMI_PON_BASE + 0x55))
PON_PMIC_WD_RESET_S2_CTL=$(($SID + $SPMI_PON_BASE + 0x56))
PON_PMIC_WD_RESET_S2_CTL2=$(($SID + $SPMI_PON_BASE + 0x57))
PON_PMIC_WD_RESET_PET=$(($SID + $SPMI_PON_BASE + 0x58))
PON_WATCHDOG_LOCK=$(($SID + $SPMI_PON_BASE + 0x83))

pmic_wdog_timer_reset()
{
	echo "Kernel test: starting pmic wdog timer reset\n"
	#unlock watchdog
	echo $PON_WATCHDOG_LOCK > $SPMI_PON_PATH/address
	echo 0x00 > $SPMI_PON_PATH/data
	#disable pmic wdog reset logic
	echo $PON_PMIC_WD_RESET_S2_CTL2 > $SPMI_PON_PATH/address
	echo 0x00 > $SPMI_PON_PATH/data
	#configure S1 and S2 timer
	echo $PON_PMIC_WD_RESET_S1_TIMER > $SPMI_PON_PATH/address
	echo 0x1 0x1 0x1 > $SPMI_PON_PATH/data
	sleep 3
	#clear pmic wdog timer to zero
	echo $PON_PMIC_WD_RESET_PET > $SPMI_PON_PATH/address
	echo 0x1 > $SPMI_PON_PATH/data
	#re-enable pmic wdog reset logic
	echo $PON_PMIC_WD_RESET_S2_CTL2 > $SPMI_PON_PATH/address
	echo 0x80 > $SPMI_PON_PATH/data
	#lock watchdog
	echo $PON_WATCHDOG_LOCK > $SPMI_PON_PATH/address
	echo 0x80 > $SPMI_PON_PATH/data
	sleep 5
	echo "Kernel test: PMIC wdog timer reset failed\n"
}

PON_KPDPWR_N_RESET_S1_TIMER=$(($SID + $SPMI_PON_BASE + 0x40))
PON_KPDPWR_N_RESET_S2_TIMER=$(($SID + $SPMI_PON_BASE + 0x41))
PON_KPDPWR_N_RESET_S2_CTL=$(($SID + $SPMI_PON_BASE + 0x42))
PON_KPDPWR_N_RESET_S2_CTL2=$(($SID + $SPMI_PON_BASE + 0x43))

extended_power_key_reset()
{
	echo "Kernel test: starting power key reset\n"
	echo $PON_KPDPWR_N_RESET_S2_CTL2 > $SPMI_PON_PATH/address
	echo 0x00 > $SPMI_PON_PATH/data

	echo $PON_KPDPWR_N_RESET_S1_TIMER > $SPMI_PON_PATH/address
	# x1 = 1352ms
	# x2 = 2s
	echo 0x0A 0x7 0x1 > $SPMI_PON_PATH/data

	sleep 3

	echo $PON_KPDPWR_N_RESET_S2_CTL2 > $SPMI_PON_PATH/address
	echo 0x80 > $SPMI_PON_PATH/data
	echo "Please press power key for at least 3 seconds\n"

	sleep 10

	echo "Kernel test: Power key reset failed\n"
}

while [ $# -gt 0 ]
do
	case $1 in
	-s | --pmic-software-reset)
		software_reset_sequence
		exit 1
	;;
	-w | --pmic-wdt-reset)
		pmic_wdog_timer_reset
		exit 1
	;;
	-p | --power-key-reset)
		extended_power_key_reset
		exit 1
	;;
	-t | --tsensor-reset)
		echo "kernel test: starting tsensor reset\n"
		if [ -e $tsensor_reset_mod ]; then
			insmod $tsensor_reset_mod tsense_reset=1
		else
			echo "Kernel test: tsensor module not found\n"
		fi
		sleep 2
		echo "Kernl test: tsensor reset failed\n"
		exit 1
	;;
	-h | --help | *)
		echo "Usage: $0 [-s | --pmic-software-reset] [-w | --pmic-wdt-reset] \\"
		echo "       [-p | --power-key-reset] [-t | --tsensor-reset] [-h | --help]"
		echo "For example: abnormal_reset.sh -s"
		exit 1
	;;
	esac
done
