/**
 * @file manager_msgr.h
 * @brief Quectel IPC public declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __MANAGER_MSGR_H__
#define __MANAGER_MSGR_H__

typedef enum {
	WIFI_IDX_AP_0 = 1,
	WIFI_IDX_AP_1,
	WIFI_IDX_STA_0
} wifi_index_e;

typedef enum {
	WIFI_WORK_MODE_AP = 0,
	WIFI_WORK_MODE_STA,
	WIFI_WORK_MODE_AP_STA,
	WIFI_WORK_MODE_AP_AP,
	WIFI_WORK_MODE_MAX
} wifi_work_mode_e;

typedef enum {
	WIFI_START = 0,
	WIFI_STOP,
	WIFI_RESTART,
} wifi_active_e;

typedef enum {
	WIFI_MODE_11A = 0,
	WIFI_MODE_11AN,
	WIFI_MODE_11B,
	WIFI_MODE_11BG,
	WIFI_MODE_11BGN,
	WIFI_MODE_11AC
} wifi_ieee80211_mode_e;

typedef enum {
	WIFI_BANDWIDTH_20MHZ = 0,
	WIFI_BANDWIDTH_40MHZ,
	WIFI_BANDWIDTH_80MHZ,
	WIFI_BANDWIDTH_160MHZ,
	WIFI_BANDWIDTH_MAX,
} wifi_bandwidth_e;

typedef enum {
	WIFI_AUTH_OPEN = 0,
	WIFI_AUTH_WEP,
	WIFI_AUTH_WPA_PSK,
	WIFI_AUTH_WPA2_PSK,
	WIFI_AUTH_WPA_WPA2_PSK_BOTH,
	WIFI_AUTH_WPA,
	WIFI_AUTH_WPA2,
	WIFI_AUTH_WPA_WPA2_BOTH,
	WIFI_AUTH_WPS
} wifi_auth_e;

typedef enum {
	WIFI_AUTH_WPA_PAIRWISE_TKIP = 0,
	WIFI_AUTH_WPA_PAIRWISE_AES,
	WIFI_AUTH_WPA_PAIRWISE_BOTH
} wifi_wpa_pairwise_e;

typedef enum {
	WIFI_SSID_HIDE_DISABLE = 0,
	WIFI_SSID_HIDE_ENABLE
} wifi_ssid_hide_state_e;

typedef enum {
	WIFI_11N_ONLY_DISABLE = 0,
	WIFI_11N_ONLY_ENABLE
} wifi_11n_only_state_e;

typedef enum {
	WIFI_11AC_ONLY_DISABLE = 0,
	WIFI_11AC_ONLY_ENABLE
} wifi_11ac_only_state_e;

typedef enum {
	WIFI_AP_ISOLATE_DISABLE = 0,
	WIFI_AP_ISOLATE_ENABLE
} wifi_ap_isolate_state_e;

typedef enum {
	WIFI_DFS_DISABLE = 0,
	WIFI_DFS_ENABLE
} wifi_dfs_state_e;

typedef enum {
	WIFI_RSSI_MIN = 0,
	WIFI_RSSI_MAX = 95
} wifi_rssi_e;

typedef enum {
	WIFI_ENABLE_SUCCESS = 0,          /*<! Enable WiFi success */
	WIFI_ENABLE_FAILED_DRIVER,        /*<! Enable WiFi failure by driver install module failed */
	WIFI_ENABLE_FAILED_SOFTWARE,      /*<! Enable WiFi failure by config error */
	WIFI_ENABLE_FAILED_NOT_SUPPORT    /*<! Enable WiFi failure by other reason */
} wifi_enable_state_e;

typedef enum {
	DATA_CALL_PROFILE_TYPE_3GPP = 0,           /*<! 3GPP */
	DATA_CALL_PROFILE_TYPE_3GPP2,              /*<! 3GPP2 */
	DATA_CALL_PROFILE_TYPE_EPC,                /*<! EPC */
	DATA_CALL_PROFILE_TYPE_INVALID = 0xff,     /*<! invalid profile type */
} data_call_profile_type_e;

typedef enum {
	DATA_CALL_IPV4 = 0,
	DATA_CALL_IPV6,
	DATA_CALL_IPV4V6,
} data_call_ip_family_e;

typedef enum {
	DATA_CALL_PDP_TYPE_IPV4 = 0,            /*<! IPv4 */
	DATA_CALL_PDP_TYPE_PPP,                 /*<! PPP */
	DATA_CALL_PDP_TYPE_IPV6,                /*<! IPv6 */
	DATA_CALL_PDP_TYPE_IPV4V6,              /*<! IPv4 and IPv6 */
	DATA_CALL_PDP_TYPE_INVALID = 0xff,      /*<! invalid pdp type */
} data_call_pdp_type_e;

typedef enum {
	DATA_CALL_AUTH_PROTOCOL_NONE = 0,
	DATA_CALL_AUTH_PROTOCOL_PAP,
	DATA_CALL_AUTH_PROTOCOL_CHAP,
	DATA_CALL_AUTH_PROTOCOL_PAP_CHAP,
	DATA_CALL_AUTH_PROTOCOL_INVALID = 0xff,
} data_call_profile_auth_protocol_type_e;

typedef enum {
	DATA_CALL_QCMAP_TECH_PREF_ANY = 0,
	DATA_CALL_QCMAP_TECH_PREF_UTMS,
	DATA_CALL_QCMAP_TECH_PREF_CDMA,
} data_call_qcmap_tech_pref_e;

#endif /* __MANAGER_MSGR_H__ */
