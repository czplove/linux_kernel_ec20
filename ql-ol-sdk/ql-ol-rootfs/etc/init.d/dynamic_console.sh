#!/bin/sh
# the script be to dyamic switch console and uart fuction
# when the uart is not console ,then it can be AT uart by linux ddp process
# when the uart is console ,the it cab be terminal of linux
# Author : Darren
# Date   : 2017/8/7

console_line="S:2345:respawn:/sbin/getty -L ttyHSL0 115200 console"
tabfile="/etc/inittab"

if [ ${1} = "start" ];then
	grep "${console_line}" ${tabfile}
	[ $? -ne 0 ] && stty -F /dev/ttyHSL0 -echo
	exit 0
fi

if [ ${1} -eq 1 ];then
	sed -i '/ttyHSL0/d' ${tabfile}
	stty -F /dev/ttyHSL0 -echo
fi

if [ ${1} -eq 0 ];then
	grep "${console_line}" ${tabfile}
	[ $? -ne 0 ] && echo ${console_line} >> ${tabfile}
fi
sync
sleep 1
exit 0
