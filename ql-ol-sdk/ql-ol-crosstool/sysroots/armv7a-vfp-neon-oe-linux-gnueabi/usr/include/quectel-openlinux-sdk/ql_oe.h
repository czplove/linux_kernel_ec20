#ifndef __QL_OE_H__
#define __QL_OE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <dirent.h>
#include <comdef.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>
#include <malloc.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <locale.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

// qmi-related
#include <comdef.h>
#include <qmi/qmi.h>
#include <qmi/qmi_port_defs.h>
#include <qmi/wireless_data_service_v01.h>
#include <qmi/device_management_service_v01.h>
#include <qmi/network_access_service_v01.h>
#include <qmi/user_identity_module_v01.h>
#include <qmi/wireless_messaging_service_v01.h>
#include <qmi/wireless_data_service_v01.h>
#include <qmi/voice_service_v02.h>
#include <qmi-framework/qmi_client.h>
#include <qmi-framework/qmi_cci_target_ext.h>
#include <qmi-framework/qmi_idl_lib.h>
#include <qmi-framework/qmi_client_instance_defs.h>
#include <qmi/quec_common_qmi_v01.h>

// dsi_netctrl
#include <dsutils/ds_util.h>
#include <data/dsi_netctrl.h>

// mcm related
#include "mcm_sms_v01.h"
#include "mcm_atcop_v01.h"
#include "mcm_client_v01.h"
#include "mcm_client.h"
#include "mcm_dm_v01.h"
#include "mcm_nw_v01.h"
#include "mcm_voice_v01.h"
#include "mcm_mobileap_v01.h"
#include "mcm_data_v01.h"
#include "mcm_sim_v01.h"
#include "mcm_loc_v01.h"

#include "quectel-features-config.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "ql_eint.h"
#include "ql_i2c.h"
#include "ql_audio.h"
#include "ql_uart.h"
#include "ql_wwan.h"
//#include "atchannel.h"
#include "ql_at.h"
#include "ql_adc.h"
#include "ql_dev.h"
#include "ql_sim.h"
#include "ql_timer.h"
#include "ql_nw.h"
#include "ql_vcall.h"
#include "ql_quecIO.h"
#include "ql_sleep_wakelock.h"
#include "ql_nslookup.h"
#include "ql_wifi.h"
#include "ql_sgmii.h"
#include "ql_nand.h"
#include "ql_wwan_v2.h"
#include "ql_mcm.h"
#include "ql_mcm_data.h"
#include "ql_mcm_mobap.h"
#include "ql_mcm_sim.h"
#include "ql_mcm_nw.h"
#include "ql_mcm_gps.h"
#include "ql_mcm_atc.h"
#include "ql_mcm_sms.h"
#include "ql_mcm_voice.h"
#include "ql_mcm_dm.h"
#include "ql_acdb.h"
#include "ql_time.h"
#include "ql_ntp.h"
#include "ql_voice_record.h"
#include "ql_utils.h"


#ifndef POLLRDHUP
#define POLLRDHUP       0x2000
#endif

#ifndef boolean
#define boolean unsigned int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define QL_USER_LOG  printf

#ifdef __cplusplus
}
#endif

#endif	//__QL_OE_H__
