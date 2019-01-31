#!/bin/sh
low_mem=2800
restart=0
restartmax_low=10
restartmax_high=20
while [ "1" = "1" ]
do
ps -fe | grep portal_gateway | grep -v grep
if [ $? -ne 0 ]
then
   exit
fi
mem_free=`free  | grep Mem | awk '{print  $4}'`
if [ $mem_free -lt $low_mem ]
then
	if [ $restart -gt $restartmax_high ]
	then
		restart=0
		pkill -9 QCMAP_ConnectionManager
		pkill -9 dnsmasq
		pkill -9 udhcpc
		pkill -9 hostapd
		pkill cxmapp
		sleep 1
		start-stop-daemon -S -b -a QCMAP_ConnectionManager /etc/mobileap_cfg.xml	
	fi

	if [ $restart -gt $restartmax_low ]
	then
	/usr/bin/portal_ctl save
	pkill portal_gateway
	sleep 1
	/usr/bin/portal_gateway -d 0
	fi

	echo 3 >/proc/sys/vm/drop_caches
	restart=$(($restart + 1))
	echo $restart > /etc/portal_restart_cnt
fi
sleep 10
done
