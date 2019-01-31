#export PATH=/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools:$PATH
#export OUT_HOST_ROOT=/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools
#/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools/ota_from_target_files -p $OUT_HOST_ROOT -f 1  -v $1 update.zip
export PATH=.:$PATH
export OUT_HOST_ROOT=.

sudo python build.py
