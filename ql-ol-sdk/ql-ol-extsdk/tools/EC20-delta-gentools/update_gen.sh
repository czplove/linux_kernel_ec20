#export PATH=/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools:$PATH
#export OUT_HOST_ROOT=/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools
#/home/chris/EC25/EC25_R01_0304/MDM9x07/MCU_R01/apps_proc/bootable/scripts/tools/releasetools/ota_from_target_files -p $OUT_HOST_ROOT -f 1  -v $1 update.zip
export PATH=.:$PATH
export OUT_HOST_ROOT=.

case $1 in
	"m" | "l" | "o" | "a" | "mo" | "lo")
#./ota_from_target_files -p . -s "../../../../device/qcom/common" -f 1 -v $1 update.zip
#./ota_from_target_files -p . -s "../../../../device/qcom/common", -d MTD -v -i ./v1/targetfiles.zip  ./v2/targetfiles.zip update.zip
	./ota_from_target_files  -p . -s "../../../../device/qcom/common", -t $1 -v -d MTD -v -i ./v1/targetfiles.zip  ./v2/targetfiles.zip update.zip
	;;
	*)
	echo "Parameter input errors !!!"
	echo "Input parameters such as: < m --modem, l --linux, o --boot, a --all >"  
	;;
esac
