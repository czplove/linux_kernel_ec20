# cywdhd
#####################
# SDIO Basic feature
#####################


DHDCFLAGS += -Wall -Wstrict-prototypes -DLINUX -Dlinux -DBCMDRIVER \
          -DBCMDONGLEHOST -DUNRELEASEDCHIP -DBCMDMA32 -DBCMFILEIMAGE -DCUSTOMER_IPCAM \
          -DBDC -DTOE -DDHD_BCMEVENTS \
          -DSHOW_EVENTS -DPROP_TXSTATUS -DLIMIT_BORROW -DBCMSDIO -DBCMLXSDMMC \
          -DBCMSDIOH_TXGLOM -DARP_OFFLOAD_SUPPORT -DOEM_ANDROID -DBCMDBG -DCUSTOMER_HW2 \
          -DEMBEDDED_PLATFORM -DKEEP_ALIVE -DWL11ULB -DPKT_FILTER_SUPPORT -DWL_VIRTUAL_APSTA \
          -DWLAIBSS -DWLAIBSS_PS -DWL_RELMCAST -DWL_NAN -DBT_WIFI_HANDOVER \
          -DWL_SUPPORT_AUTO_CHANNEL -DIPV6_NDO_SUPPORT -DPNO_SUPPORT -DTSQ_MULTIPLIER=10 \
          -DCSCAN -DWL11U -DWL_CFG80211 -DWLP2P -DWLTDLS -DESCAN_RESULT_PATCH \
          -DMFP -DWL_IFACE_COMB_NUM_CHANNELS -DHT40_GO \
          -DWL_SDO -DVSDB -DWL_CFG80211_VSDB_PRIORITIZE_SCAN_REQUEST \
          -DWL_CFG80211_STA_EVENT -DSUPPORT_PM2_ONLY -DAMPDU_HOSTREORDER -DDHD_USE_IDLECOUNT \
          -DSET_RANDOM_MAC_SOFTAP -DWIFI_ACT_FRAME -D__KERNEL__ -DSDIO_ISR_THREAD -DWL_WIRELESS_EXT -DWLLXIW -DUSE_IW

#DHDCFLAGS += -DDHD_DEBUG -DDHD_FW_COREDUMP -DSDTEST -DDHD_IFDEBUG


##########################
# driver type
# m: module type driver
# y: built-in type driver
##########################
DRIVER_TYPE ?= $(CONFIG_BCMDHD)
 
DHDCFLAGS += -DSUSPEND16    

        

EXTRA_CFLAGS += $(DHDCFLAGS) 
EXTRA_CFLAGS += -DSRCBASE=\"$(src)\"
EXTRA_CFLAGS += -I$(src)/include/ -I$(src)/
KBUILD_CFLAGS += -I$(LINUXDIR)/include -I$(shell pwd)

DHDOFILES := dhd_linux.o dhd_linux_sched.o linux_osl.o bcmutils.o hnd_pktq.o \
          hnd_pktpool.o bcmevent.o dhd_common.o dhd_ip.o dhd_linux_platdev.o \
          dhd_linux_wq.o dhd_debug.o bcmxtlv.o bcmwifi_channels.o dhd_custom_gpio.o \
          siutils.o sbutils.o aiutils.o hndpmu.o dhd_cdc.o dhd_wlfc.o dhd_sdio.o \
          bcmsdh_sdmmc.o bcmsdh.o bcmsdh_linux.o bcmsdh_sdmmc_linux.o dhd_macdbg.o \
          dhd_pno.o wl_android.o wldev_common.o wl_cfgnan.o dhd_debug_linux.o \
          wl_cfg80211.o wl_cfgp2p.o dhd_cfg80211.o wl_cfg_btcoex.o wl_cfgvendor.o \
          wl_linux_mon.o bcm_app_utils.o wl_iw.o


cywdhd-objs := $(DHDOFILES)
obj-$(DRIVER_TYPE)   += cywdhd.o

all:
	@echo "$(MAKE) --no-print-directory -C $(KDIR) SUBDIRS=$(CURDIR) modules"
	@$(MAKE) --no-print-directory -C $(KDIR) SUBDIRS=$(CURDIR) modules

clean:
	rm -rf *.o *.ko *.mod.c *~ .*.cmd *.o.cmd .*.o.cmd \
	Module.symvers modules.order .tmp_versions modules.builtin

install:
	@$(MAKE) --no-print-directory -C $(KDIR) \
		SUBDIRS=$(CURDIR) modules_install
