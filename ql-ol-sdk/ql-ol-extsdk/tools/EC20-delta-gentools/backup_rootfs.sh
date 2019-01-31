#!/bin/sh
# mkfs.ubifs and ubinize is ubifs tools to make ubi image that contain ubifs and ubi volume.
# mkfs.ubifs make a UBIFS file system image from an existing directory tree
# ubinize make all ubi volume for UBIFS file system image which have to define in input file *.cfg
# mkfs.ubifs Options:
# -m : minimum I/O unit size(pagesize of a flash)
# -e : logical erase block size(physical eraseblock - 2*pagesize )
# -c : maximum logical erase block count(must be greater than volume size in BLOCK)
# ubinize Options:
# -p : physical eraseblock size(physical eraseblock of a flash)
# -m : same as mkfs.ubifs -m 
# -s : subpage size,we set the value is equal pagesize
# Author : Darren
# Data : 2017/7/24

ubisize='' #size of backuprootfs
mtd_block_number='' #the backup partition of rootfs
mtd_file='/proc/mtd'
fifo_file='/dev/null'
lebsize=''
pebsize=''
pagesize=''
subpagesize=''
BackupRootfs()
{
	rm -fr /dev/shm/cache #delete file that recovery unzip update.zip to /dev/shm
	mkdir -p /data/backup_rootfs/data/configs
	mkdir -p /data/backup_rootfs/data/logs

	mkfs.ubifs -m ${pagesize} -e ${lebsize}  -c 2146 -r /data/backup_rootfs/data/ -F -o /dev/shm/linux-usrfs.ubifs
	[ $? -ne 0 ] && exit 2 || echo "/dev/shm/linux-usrfs.ubifs create sucessfully"
	echo "progress:5%" > ${1}
	mkfs.ubifs -m ${pagesize} -e ${lebsize}  -c 2146 -r /system -F -o /dev/shm/linux-rootfs.ubifs
	[ $? -ne 0 ] && exit 3 || echo "/dev/shm/linux-rootfs.ubifs create sucessfully"
	echo "progress:55%" > ${1}
	ubinize -o /cache/linux-sysfs.ubi -p ${pebsize} -m ${pagesize} -s ${pagesize} /etc/ubinize_system_rootfs.cfg
	[ $? -ne 0 ] && exit 4 || echo "/cache/linux-sysfs.ubi create sucessfully"
	echo "progress:100%" > ${1}
	rm -fr /dev/shm/linux-rootfs.ubifs /dev/shm/linux-usrfs.ubifs
	rm -fr /data/backup_rootfs/
}

WriteDataToSysback()
{
	[ $# -eq 1 ] && partitionsName=${1} || partitionsName='sys_back'
	ubisize=`stat /cache/linux-sysfs.ubi | awk '/Size/ {print $2}'`
	ubisize=$((ubisize/262144+1)) #262144 = 256KB，flash minIOsize
	echo "linux-sysfs.ubi size = ${ubisize}"
	mtd_block_number=`cat $mtd_file | grep -i "${partitionsName}" | sed 's/^mtd//' | awk -F ':' '{print $1}'`

	dd if=/cache/linux-sysfs.ubi of=/dev/mtd${mtd_block_number} bs=256k count=${ubisize}
	[ $? -ne 0 ] && exit 5 || echo "write data to sys_back sucessfully"
}
echo "backup uibfs start..."
[ $# -eq 1 ] && fifo_file=${1}
echo "fifo_file:${1}"
lebsize=`ubinfo /dev/ubi0 | grep "Logical eraseblock size" | awk '{print $4}'`
pagesize=`ubinfo /dev/ubi0 | grep "Minimum input/output unit" | awk '{print $5}'`
pebsize=$((lebsize + pagesize*2))
BackupRootfs ${fifo_file}
# WriteDataToSysback sys_back


