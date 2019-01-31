# Makefile for QuecOpen SDK
#
# Copyright (C) 2017 Quectel
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

TOPDIR:=$(shell pwd)
ABOOT_DIR:=$(TOPDIR)/ql-ol-bootloader
KERNEL_DIR:=$(TOPDIR)/ql-ol-kernel
ROOTFS_DIR:=$(TOPDIR)/ql-ol-rootfs
EXTSDK_DIR:=$(TOPDIR)/ql-ol-extsdk
#export TOPDIR

empty:=
space:= $(empty) $(empty)
$(if $(findstring $(space),$(TOPDIR)),$(error ERROR: The path to the work directory must not include any spaces))

crosstool=crosstool
aboot=aboot
kernel_menuconfig=kernel_menuconfig
debug_kernel_menuconfig=debug_kernel_menuconfig
kernel=kernel
kernel_module=kernel_module
rootfs=rootfs
usrdata=usrdata
extsdk=extsdk
debug_version=debug_version
#single=$(shell ${#1##*/})
pagesize:=4096
leb-size:=253952
max-leb-cnt:=2146
peb-size:=256KiB
ABOOT_MAKE_FLAGS_EXTRA:=
ubicfg-fname:=ubinize_system_userdata.cfg
sysfs-ubi-fname:=mdm9607-perf-sysfs.ubi
has_usrfs:=false

ifneq ($(filter $(QUECTEL_PROJECT_NAME), AG35C AG35CE), )
	KERNEL_DIR:=$(KERNEL_DIR)/msm-3.18
	pagesize=2048
	peb-size=128KiB
	leb-size=126976
	max-leb-cnt=4292
	ABOOT_MAKE_FLAGS_EXTRA+=TOOLCHAIN_PREFIX=arm-oe-linux-gnueabi- LIBGCC=${LIBGCC} SIGNED_KERNEL=1
	ubicfg-fname=ubinize_system_userdata.cfg
	sysfs-ubi-fname=mdm9607-sysfs.ubi
	has_usrfs=true
else ifneq ($(filter $(QUECTEL_PROJECT_NAME), AG35CEN AG35CEVBM), )
	KERNEL_DIR:=$(KERNEL_DIR)/msm-3.18
	pagesize=4096
	peb-size=256KiB
	leb-size=253952
	max-leb-cnt=2146
	ABOOT_MAKE_FLAGS_EXTRA+=TOOLCHAIN_PREFIX=arm-oe-linux-gnueabi- LIBGCC=${LIBGCC} SIGNED_KERNEL=1
	ubicfg-fname=ubinize_system_userdata.cfg
	sysfs-ubi-fname=mdm9607-sysfs.ubi
	has_usrfs=true
else ifneq ($(filter $(QUECTEL_PROJECT_NAME),EC20C_HA EC20C_HB  EC20CET EC20CEL EC20C_FT EC20C_FTB EC20CE_FBKG), )
	pagesize=2048
	peb-size=128KiB
	leb-size=126976
	max-leb-cnt=4292
	ABOOT_MAKE_FLAGS_EXTRA+=TOOLCHAIN_PREFIX=arm-oe-linux-gnueabi- SIGNED_KERNEL=1
	ubicfg-fname=ubinize.cfg
	sysfs-ubi-fname=mdm9607-perf-sysfs.ubi
else
	pagesize=4096
	peb-size=256KiB
	leb-size=253952
	max-leb-cnt=2146
	ABOOT_MAKE_FLAGS_EXTRA+=TOOLCHAIN_PREFIX=arm-oe-linux-gnueabi- SIGNED_KERNEL=1
	ubicfg-fname=ubinize.cfg
	sysfs-ubi-fname=mdm9607-perf-sysfs.ubi
endif

#$(shell source ql-ol-crosstool/ql-ol-crosstool-env-init)
$(shell mkdir -p target)

world : $(crosstool) $(kernel) $(kernel_module) $(aboot) $(rootfs) $(usrdata)

$(crosstool):
ifneq ($(CROSSTOOL_FLAG), 1)
	@echo -e "\033[31m###### Please exec cmd to init crosstool: source ql-ol-crosstool/ql-ol-crosstool-env-init #######\033[0m"
	@exit 1
else
	@echo -e "\033[32m########## crosstool have inited previously #########\033[0m"
endif

$(aboot):
	cd $(ABOOT_DIR) ; make -j 4 mdm9607 $(ABOOT_MAKE_FLAGS_EXTRA) || exit ; \
	cp build-mdm9607/appsboot.mbn $(TOPDIR)/target

$(kernel_menuconfig):
	cd $(KERNEL_DIR) ; make ARCH=arm mdm9607-perf_defconfig menuconfig O=build ;

$(debug_kernel_menuconfig):
	cd $(KERNEL_DIR) ; make ARCH=arm mdm9607_defconfig menuconfig O=build ;

$(kernel):
	cd $(KERNEL_DIR) ; [ ! -f build/.config ] && echo -e "\033[31m.config is nonexist, Please exec \"make kernel_menuconfig\" \033[0m" && exit 1 ;\
	make ARCH=arm CC=arm-oe-linux-gnueabi-gcc LD=arm-oe-linux-gnueabi-ld.bfd -j 4 O=build || exit ; \
	cp build/arch/arm/boot/zImage build/arch/arm/boot/dts/qcom/mdm9607-mtp.dtb $(EXTSDK_DIR)/tools/quectel_mkboot/
	cd $(EXTSDK_DIR)/tools/quectel_mkboot ; chmod +x * ; ./quec_mkboot dtb2img mdm9607-mtp.dtb $(QUECTEL_PROJECT_NAME) ; \
	cp ./target/* $(TOPDIR)/target

$(kernel_module):
	cd $(KERNEL_DIR) ; [ ! -f build/.config ] && echo -e "\033[31m.config is nonexist, Please exec \"make kernel_menuconfig\" \033[0m" && exit 1; \
	make modules ARCH=arm CC=arm-oe-linux-gnueabi-gcc LD=arm-oe-linux-gnueabi-ld.bfd -j 4 O=build || exit ; \
	make ARCH=arm CROSS_COMPILE=arm-oe-linux-gnueabi- INSTALL_MOD_STRIP=1 O=$(KERNEL_DIR)/build INSTALL_MOD_PATH=$(ROOTFS_DIR)/usr  modules_install || exit
	echo -e "\033[32m###### kernel module compile completely, need make rootfs to generate sysfs.ubi again! ######\033[0m"

$(rootfs):
	cd $(TOPDIR) ; chmod +x ./ql-ol-extsdk/tools/quectel_ubi/* ; \
	./ql-ol-extsdk/tools/quectel_ubi/mkfs.ubifs -r ql-ol-rootfs -o mdm9607-perf-sysfs.ubifs -m $(pagesize) -e $(leb-size) -c $(max-leb-cnt) -F
ifeq ($(has_usrfs),true)
	./ql-ol-extsdk/tools/quectel_ubi/mkfs.ubifs -r ql-ol-usrfs -o mdm9607-usrfs.ubifs -m $(pagesize) -e $(leb-size) -c $(max-leb-cnt) -F
endif
	./ql-ol-extsdk/tools/quectel_ubi/ubinize  -o $(sysfs-ubi-fname) -m $(pagesize) -p $(peb-size) -s $(pagesize) ql-ol-extsdk/tools/quectel_ubi/$(ubicfg-fname)
	mv mdm9607-perf-*.ubifs $(sysfs-ubi-fname) target/

$(usrdata):
	cd $(TOPDIR) ; chmod +x ./ql-ol-extsdk/tools/quectel_ubi/* ; ./ql-ol-extsdk/tools/quectel_ubi/mkfs.ubifs -r ql-ol-usrdata -o usrdata.ubifs -m $(pagesize) -e $(leb-size) -c $(max-leb-cnt) -F ;	\
	./ql-ol-extsdk/tools/quectel_ubi/ubinize  -o usrdata.ubi -m $(pagesize) -p $(peb-size) -s $(pagesize) ql-ol-extsdk/tools/quectel_ubi/ubinize_usrdata.cfg ; \
	mv usrdata.ubifs usrdata.ubi target/

$(customapps):
	cd $(TOPDIR) ; chmod +x ./ql-ol-extsdk/tools/quectel_ubi/* ; ./ql-ol-extsdk/tools/quectel_ubi/mkfs.ubifs -r customapps -o customapps.ubifs -m $(pagesize) -e $(leb-size) -c $(max-leb-cnt) -F ;	\
	./ql-ol-extsdk/tools/quectel_ubi/ubinize  -o customapps.ubi -m $(pagesize) -p $(peb-size) -s $(pagesize) ql-ol-extsdk/tools/quectel_ubi/ubinize_customapps.cfg ; \
	mv customapps.ubifs customapps.ubi target/

$(debug_version): kernel kernel_module rootfs

$(extsdk):
	cd $(EXTSDK_DIR)/example ; make clean || exit ; make || exit

clean: aboot/clean kernel/clean rootfs/clean extsdk/clean usrdata/clean

aboot/clean:
	cd $(ABOOT_DIR) ; make -j 4 mdm9607 clean ; rm -rf build-mdm9607
	rm -rf target/appsboot.mbn

kernel/clean:
	cd $(KERNEL_DIR) ; make clean O=build || exit
	rm -rf target/*.img

kernel/distclean:
	cd $(KERNEL_DIR) ; make distclean O=build || exit
	rm -rf target/*.img

rootfs/clean:
	rm -rf target/mdm9607-perf-sysfs.ubi*

usrdata/clean:
	rm -rf target/usrdata.ubi*

extsdk/clean:
	cd $(EXTSDK_DIR)/example ; make clean || exit

