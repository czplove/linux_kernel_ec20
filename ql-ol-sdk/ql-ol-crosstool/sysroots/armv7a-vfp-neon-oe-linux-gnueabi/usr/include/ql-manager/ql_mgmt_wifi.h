/**
 * @file ql_mgtm_wifi.h
 * @brief Quectel WiFi interface function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_MGMT_WIFI_H__
#define __QL_MGMT_WIFI_H__

#include "manager_msgr.h"

typedef enum {
	QL_MGMT_CLIENT_COMMAND_WIFI_WORK_MODE                          = 0x0001,
	QL_MGMT_CLIENT_COMMAND_WIFI_COUNTRY_CODE                       = 0x0002,
	QL_MGMT_CLIENT_COMMAND_WIFI_MODE                               = 0x0003,
	QL_MGMT_CLIENT_COMMAND_WIFI_CHANNEL                            = 0x0004,
	QL_MGMT_CLIENT_COMMAND_WIFI_SSID                               = 0x0005,
	QL_MGMT_CLIENT_COMMAND_WIFI_SSID_HIDE                          = 0x0006,
	QL_MGMT_CLIENT_COMMAND_WIFI_BANDWIDTH                          = 0x0007,
	QL_MGMT_CLIENT_COMMAND_WIFI_MAX_STA                            = 0x0008,
	QL_MGMT_CLIENT_COMMAND_WIFI_MAC_BLACKWHITE_LIST                = 0x0009,
	QL_MGMT_CLIENT_COMMAND_WIFI_11N_ONLY                           = 0x000a,
	QL_MGMT_CLIENT_COMMAND_WIFI_11AC_ONLY                          = 0x000b,
	QL_MGMT_CLIENT_COMMAND_WIFI_AP_ISOLATE                         = 0x000c,
	QL_MGMT_CLIENT_COMMAND_WIFI_DFS                                = 0x000d,
	QL_MGMT_CLIENT_COMMAND_WIFI_AUTH                               = 0x000e,
	QL_MGMT_CLIENT_COMMAND_WIFI_RSSI                               = 0x000f,
	QL_MGMT_CLIENT_COMMAND_WIFI_STA_INFO                           = 0x0010,
	QL_MGMT_CLIENT_COMMAND_WIFI_ENABLE                             = 0x0011,
	QL_MGMT_CLIENT_COMMAND_WIFI_ACTIVE                             = 0x0012,
} ql_mgmt_client_command_wifi_e;

#define QL_WIFI_MAX_STA_NUM 16

struct ql_mgmt_client_wifi_work_mode {
	/*
	 * WiFi work mode support AP, STA, AP+STA, AP-AP
	 */
	wifi_work_mode_e work_mode;
};

struct ql_mgmt_client_wifi_enable {
	bool state;
};

struct ql_mgmt_client_wifi_country_code {
	char country_code[2];
};

struct ql_mgmt_client_wifi_mode {
	wifi_index_e id;
	wifi_ieee80211_mode_e mode;
};

struct ql_mgmt_client_wifi_channel {
	wifi_index_e id;
	int channel;
};

struct ql_mgmt_client_wifi_essid {
	wifi_index_e id;
	char ssid[32];
};

struct ql_mgmt_client_wifi_ssid_hide {
	wifi_index_e id;
	wifi_ssid_hide_state_e state;
};

struct ql_mgmt_client_wifi_bandwidth {
	wifi_index_e id;
	wifi_bandwidth_e bandwidth;
};

struct ql_mgmt_client_wifi_max_sta {
	wifi_index_e id;
	int num;
};

struct ql_mgmt_client_wifi_11n_only {
	wifi_index_e id;
	wifi_11n_only_state_e state;
};

struct ql_mgmt_clinet_wifi_11ac_only {
	wifi_index_e id;
	wifi_11ac_only_state_e state;
};

struct ql_mgmt_client_wifi_ap_isolate {
	wifi_index_e id;
	wifi_ap_isolate_state_e state;
};

struct ql_mgmt_client_wifi_dfs {
	wifi_index_e id;
	wifi_dfs_state_e state;
};

struct ql_mgmt_client_wifi_auth {
	wifi_index_e id;
	wifi_auth_e auth;
	union {
		struct {
			int default_index;
			char password[4][64];
		} wep;
		struct {
			wifi_wpa_pairwise_e pairwise;
			char password[64];
			int group_rekey;
		} wpa_psk;
	};
};

struct ql_mgmt_client_wifi_rssi {
	wifi_index_e id;
	int rssi;
};

struct ql_mgmt_client_wifi_sta_info {
	int count;
	struct {
		int ip;
		char mac[6];
		char hostname[32];
		char ssid[32];
		int uptime;
		long long tx_bytes;
		long long rx_bytes;
	} sta[QL_WIFI_MAX_STA_NUM];
};

typedef struct {
	union {
		struct ql_mgmt_client_wifi_work_mode work_mode;
		struct ql_mgmt_client_wifi_enable enable;
		struct ql_mgmt_client_wifi_country_code country_code;
		struct ql_mgmt_client_wifi_mode mode;
		struct ql_mgmt_client_wifi_channel channel;
		struct ql_mgmt_client_wifi_essid ssid;
		struct ql_mgmt_client_wifi_ssid_hide ssid_hide;
		struct ql_mgmt_client_wifi_bandwidth bandwidth;
		struct ql_mgmt_client_wifi_max_sta max_sta;
		//struct ql_mgmt_client_wifi_mac_blackwhite_list blackwhite_list;
		struct ql_mgmt_client_wifi_11n_only only_11n;
		struct ql_mgmt_clinet_wifi_11ac_only only_11ac;
		struct ql_mgmt_client_wifi_ap_isolate ap_isolate;
		struct ql_mgmt_client_wifi_dfs dfs;
		struct ql_mgmt_client_wifi_auth auth;
		struct ql_mgmt_client_wifi_rssi rssi;
		struct ql_mgmt_client_wifi_sta_info sta_info;
	};
} ql_mgmt_client_wifi_s;

#endif /* end of  __QL_MGMT_WIFI_H__ */
