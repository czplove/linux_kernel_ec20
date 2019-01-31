#!/bin/sh
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
#
# find_partitions        init.d script to dynamically find partitions
#

FindAndMountUBI () {

# quectel modification 20160802 add  modem startup in the kernel
# if define(CONFIG_QUECTEL_MODEM_BOOT_TIME) && define (QUECTEL_MODEM_BACKUP), mark the follow script,it is not need now 
if false ;then
   partition=$1
   dir=$2

  mtd_block_number=`cat $mtd_file | grep -i $partition | sed 's/^mtd//' | awk -F ':' '{print $1}'`
   echo "MTD : Detected block device : $dir for $partition"
   mkdir -p $dir

  ubiattach -m $mtd_block_number -d 1 /dev/ubi_ctrl
  device=/dev/ubi1_0
  while [ 1 ]
   do
      if [ -c $device ]
      then
          mount -t ubifs -o ro /dev/ubi1_0 $dir -o bulk_read
           break
       else
          sleep 0.010
       fi
   done
fi	

# quectel modification 20160802 add start 
# 201700707  MDM9x07 OpenLinux R06  not nedd 
# quectel modification 20160802 add end
}
	
#quectel add for usr data mount 
FindAndMountUsrdataUBI () {
	partition=$1
	dir=$2
	QuecUsrdataFormatTimes=0
	Var1=0
	Var2=1
	ubiattach_time=0
	device=/dev/ubi2_0
   
   mtd_block_number=`cat $mtd_file | grep -i $partition | sed 's/^mtd//' | awk -F ':' '{print $1}'`
   echo "MTD : Detected block device : $dir for $partition"
   mkdir -p $dir

   ubiattach -m $mtd_block_number -d 2 /dev/ubi_ctrl

	while [ 1 ]
	do
		if [ -e $device ]
        then 
			break
		else
			sleep 0.01
			ubiattach_time=$((ubiattach_time+1))
			echo "ubiattach wait times= $ubiattach_time"

			if [ "$ubiattach_time" -gt "100" ]
			then
				echo " wait times = $ubiattach_time, > 100 !!!"
				break
			fi
		fi
	done

# this is usrdata partition ubi first attach , if ubiattach fail, it is mean usrdata partiotn crashed , need format
	if [ -e $device ] && [ ! -f /data/usrdata_UBIformat_flag ];then
		echo "usrdata ubi attach success !!!"
	else
		echo -n 1 > "/data/usrdata_UBIformat_flag"
		sync
		sleep 1
		ubidetach -p /dev/mtd$mtd_block_number
		sleep 1
		ubiformat /dev/mtd$mtd_block_number -y
		sleep 1
		ubiattach -p /dev/mtd$mtd_block_number
		sleep 2
		if [ -e "/dev/ubi2" ];then
			echo "ubiattach success !!!" 
		else
			echo "ubiattach failed !!!"
			sys_reboot
		fi
		sleep 1
		ubimkvol /dev/ubi2 -m -N usrdata
		sleep 1
		if [ -e "/dev/ubi2_0" ];then
			echo  "ubimkvol success !!!"
		else
			echo "ubimkvol failed !!!"
			sys_reboot
		fi
		rm -rf /data/usrdata_UBIformat_flag
		sync
		if [ -f /data/usrdata_format_info.txt ];then		
			Var1=`cat /data/usrdata_format_info.txt`
		fi
		QuecUsrdataFormatTimes=$(($Var1+$Var2))
		echo -n $QuecUsrdataFormatTimes > "/data/usrdata_format_info.txt"
		
	fi
 
	mounttimes=0
	while [ 1 ]
    do
        if [ -e $device ]
        then
            mount -t ubifs /dev/ubi2_0 $dir -o bulk_read
            break
		else
			sleep 0.01
			mounttimes=$((mounttimes+1))
			echo " $device mount times =$mounttimes"
			if [ "$mounttimes" -gt "100" ]
			then
				echo " mount times =$mounttimes, > 100.  mount failed, need format usr_data partition"
				echo -n 1 > "/data/usrdata_UBIformat_flag"
				sync
				sleep 1
				sys_reboot
			fi
		fi
    done
}

FindAndMountVolumeUBI () {
   volume_name=$1
   dir=$2
   if [ ! -d $dir ]
   then
       mkdir -p $dir
   fi
   mount -t ubifs ubi0:$volume_name $dir -o bulk_read
}

mtd_file=/proc/mtd

fstype="UBI"
eval FindAndAttachUBI modem
eval FindAndAttachUBI system
# quectel add usr_data
eval FindAndAttachUBI usr_data
eval FindAndMountUBI rootfs  /system  1
### Stanley-20170718: Remove usr_volume 
#eval FindAndMountUBI usrfs   /data    1
## Ramos no /cache volume,  fota use usrdata 
#eval FindAndMountUBI cachefs /cache
eval FindAndMountUBI usrdata /usrdata

eval FindAndMount${fstype} modem /firmware

#quectel add for usr_data partition mount 
eval FindAndMountUsrdata${fstype} usr_data /usrdata

exit 0
