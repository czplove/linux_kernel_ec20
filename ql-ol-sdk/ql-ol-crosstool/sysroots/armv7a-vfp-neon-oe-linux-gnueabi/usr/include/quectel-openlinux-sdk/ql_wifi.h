/**
 * @file ql_wifi.h
 * @brief Quectel Openlinux WiFi public function declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_WIFI_H__
#define __QL_WIFI_H__

#include <stdbool.h>
#include <netinet/in.h>

typedef enum {
	QL_WIFI_EVENT_ENABLE_STATUS = 0,
	QL_WIFI_EVENT_DISABLE_STATUS,
} ql_wifi_event_type_e;

typedef enum {
	QL_WIFI_STATUS_ENABLE = 0,
	QL_WIFI_STATUS_DISABLE,
	QL_WIFI_STATUS_ERR_DRIVER,
	QL_WIFI_STATUS_ERR_SOFTWARE,
} ql_wifi_status_e;

typedef struct {
	ql_wifi_event_type_e id;
	union {
		ql_wifi_status_e status; /* the QL_WIFI_ENABLE_STATUS or QL_WIFI_DISABLE_STATUS event */
	};
} ql_wifi_event_s;

typedef void (*wifi_event_handle)(ql_wifi_event_s *event, void *arg);

typedef enum {
	QL_WIFI_WORK_MODE_STA = 0,      /* WiFi is in STA Mode */
	QL_WIFI_WORK_MODE_AP0,          /* WiFi is in AP Mode */
	QL_WIFI_WORK_MODE_AP0_STA,      /* WiFi is in AP/STA Mode */
	QL_WIFI_WORK_MODE_AP0_AP1       /* WiFi is in AP/AP Mode */
} ql_wifi_work_mode_e;

typedef enum {
	QL_WIFI_AP_INDEX_AP0 = 0,       /* Index 0 of AP/AP-STA/AP-AP Mode */
	QL_WIFI_AP_INDEX_AP1            /* Index 1 of AP/AP-STA/AP-AP Mode */
} ql_wifi_ap_index_e;

typedef enum {
	QL_WIFI_STATION_DISABLE = 0,    /* WiFi station is not enable */
	QL_WIFI_STATION_CONNECTED,      /* WiFi station is connected hotspot */
	QL_WIFI_STATION_DISCONNECTED    /* WiFi station is disconnect hotspot */
} ql_wifi_station_status_e;

typedef enum {
	QL_WIFI_MODE_80211B = 0,        /* IEEE 802.11b (2.4 GHz) */
	QL_WIFI_MODE_80211BG,	        /* IEEE 802.11bg (2.4 GHz) */
	QL_WIFI_MODE_80211BGN,          /* IEEE 802.11bgn (2.4 GHz) */
	QL_WIFI_MODE_80211A,            /* IEEE 802.11a (5 GHz) */
	QL_WIFI_MODE_80211AN,           /* IEEE 802.11an (5 GHz) */
	QL_WIFI_MODE_80211AC            /* IEEE 802.11ac (5 GHz) */
} ql_wifi_mode_type_e;

typedef enum {
	QL_WIFI_BANDWIDTH_HT20 = 0,    
	QL_WIFI_BANDWIDTH_HT40,
	QL_WIFI_BANDWIDTH_HT80
} ql_wifi_bandwidth_type_e;

typedef enum {
	QL_WIFI_AUTH_OPEN = 0,             /* Open */
	QL_WIFI_AUTH_WPA_PSK,              /* WPA Personal */
	QL_WIFI_AUTH_WPA2_PSK,             /* WPA2 Personal */
	QL_WIFI_AUTH_WPA_WPA2_PSK_BOTH,    /* WPA&WPA2 Personal */
} ql_wifi_auth_e;

typedef enum {
	QL_WIFI_AUTH_WPA_PAIRWISE_AUTO = 0,    /* contain TKIP and AES */
	QL_WIFI_AUTH_WPA_PAIRWISE_TKIP,
	QL_WIFI_AUTH_WPA_PAIRWISE_AES
} ql_wifi_auth_wpa_psk_e;

typedef struct {
	int auth;
	union {
		struct {
			int default_index;
			char passwd[4][64];
		} wep;

		struct {
			short pairwise;
			char passwd[64];
			int group_rekey;
		} wpa_psk;
	};
} ql_wifi_ap_auth_s;

typedef struct {
	int auth;
	union {
		struct {
			char passwd[64];
		} wep;
		
		struct {
			short pairwise;
			char passwd[64];
		} wpa_psk;
	};
} ql_wifi_sta_auth_s;

typedef struct {
	int num;
	struct {
		unsigned char max[6];
		in_addr_t addr;
		char hostname[32];
	} sta[16];
} ql_wifi_ap_sta_info_s;

/**
 * Register Callbak with WiFi some evnet.
 * 
 * @param event_handle         Callbak function
 * @param arg                  User parameters
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_register_handle(wifi_event_handle event_handle, void *arg);

/**
 * Unregister Callbak with WiFi some evnet.
 * 
 * @param None
 *
 * @return 
 *   None
 * 
 */
extern void ql_wifi_unregister_handle(void);

/**
 * Enables the WLAN. This function enables the WLAN based on the hostpad or wpa_supplicant 
 * configuration provided. This function inserts the WLAN kernel module and advertises the SSID.
 * 
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_enable(void);

/**
 * Disables the WLAN. This function removes the kernel module for the Wi-Fi driver.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_disable(void);

/**
 * Create thread to enables the WLAN. This function enables the WLAN based on the hostpad or wpa_supplicant 
 * configuration provided. This function inserts the WLAN kernel module and advertises the SSID.
 * 
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_enable_async(void);

/**
 * Create thread to disables the WLAN. This function removes the kernel module for the Wi-Fi driver.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_disable_async(void);

/**
 * Inserts the WLAN kernel module.
 * 
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_module_enable(void);

/**
 * Removes the kernel module for the Wi-Fi driver.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_module_disable(void);

/**
 * Get WiFi status.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_status_get(ql_wifi_status_e *status);

/**
 * Set the WiFi configuration: WiFi work mode configuration.
 *
 * @param [in]   WiFi work mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   ql_wifi_disable is must be disable
 *
 */
extern int ql_wifi_work_mode_set(ql_wifi_work_mode_e mode);

/**
 * Get the WiFi configuration: WiFi work mode configuration.
 *
 * @param [out]   WiFi work mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_work_mode_get(ql_wifi_work_mode_e *mode);

/**
 * Set the WiFi configuration: the hotspot ssid mode configuration.
 *
 * @param [in]   Index of AP mode
 * @param [in]   the hotspot ssid name
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_ssid_set(ql_wifi_ap_index_e idx, char *ssid);

/**
 * Get the WiFi configuration: the hotspot ssid mode configuration.
 *
 * @param [in]    Index of AP mode
 * @param [out]   the hotspot ssid name
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_ssid_get(ql_wifi_ap_index_e idx, char *ssid);

/**
 * Set the WiFi configuration: hidden hotspot configuration.
 *
 * @param [in]    Index of AP mode
 * @param [in]    If set true, hidden hotspot
 *                If set false, open hotspot
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_ssid_hide_set(ql_wifi_ap_index_e idx, bool hide);

/**
 * Get the WiFi configuration: hidden hotspot configuration.
 *
 * @param [in]    Index of AP mode
 * @param [in]    Get the hotspot hidden status
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_ssid_hide_get(ql_wifi_ap_index_e idx, bool *hide);

/**
 * Set the WiFi configuration: Operation mode configuration, such as 802.11a/b/n/ac.
 *
 * @param [in]   Index of AP mode
 * @param [in]   Operation mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_mode_set(ql_wifi_ap_index_e idx, ql_wifi_mode_type_e mode);

/**
 * Get the WiFi configuration: Operation mode configuration, such as 802.11a/b/n/ac.
 *
 * @param [in]    Index of AP mode
 * @param [out]   Operation mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_mode_get(ql_wifi_ap_index_e idx, ql_wifi_mode_type_e *mode);

/**
 * Set the WiFi configuration: HT or VHT capabilities (20MHz, 40MHz, 80MHz).
 *
 * @param [in]   Index of AP mode
 * @param [in]   Bandwidth
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   The WiFi Operation mode must be set 802.11bgn/802.11an/802.11ac.
 */
extern int ql_wifi_ap_bandwidth_set(ql_wifi_ap_index_e idx, ql_wifi_bandwidth_type_e bandwidth);

/**
 * Get the WiFi configuration: HT or VHT capabilities (20MHz, 40MHz, 80MHz).
 *
 * @param [in]    Index of AP mode
 * @param [Out]   Bandwidth
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   The WiFi Operation mode must be set 802.11bgn/802.11an/802.11ac.
 */
extern int ql_wifi_ap_bandwidth_get(ql_wifi_ap_index_e idx, ql_wifi_bandwidth_type_e *bandwidth);

/**
 * Set the WiFi configuration: channel number configuration.
 *
 * @param [in]   Index of AP mode
 * @param [in]   Channel number
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   1. if WiFi set 2.4GHz, the channel list: 
 *   0/1/2/3/4/5/6/7/8/9/10/11/12/13/14
 *   2. if WiFi set 5GHz, the channel list:
 *   36/40/44/48/52/56/60/64/100/104/108/112/116/120/124/128/132/136/140/144/149/153/157/161/165/175/181 
 */
extern int ql_wifi_ap_channel_set(ql_wifi_ap_index_e idx, int channel);

/**
 * Get the WiFi configuration: channel number configuration.
 *
 * @param [in]    Index of AP mode
 * @param [out]   Channel number
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   1. if WiFi set 2.4GHz, the channel list: 
 *   0/1/2/3/4/5/6/7/8/9/10/11/12/13/14
 *   2. if WiFi set 5GHz, the channel list:
 *   36/40/44/48/52/56/60/64/100/104/108/112/116/120/124/128/132/136/140/144/149/153/157/161/165/175/181 
 */
extern int ql_wifi_ap_channel_get(ql_wifi_ap_index_e idx, int *channel);

/**
 * Set the WiFi configuration: IEEE 802.11 specifies two authentication algorithms and WPA/IEEE 802.11i configuration.
 *
 * @param [in]   Index of AP mode
 * @param [in]   authentication configuration
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_auth_set(ql_wifi_ap_index_e idx, ql_wifi_ap_auth_s *auth);

/**
 * Get the WiFi configuration: IEEE 802.11 specifies two authentication algorithms and WPA/IEEE 802.11i configuration.
 *
 * @param [in]   Index of AP mode
 * @param [out]   authentication configuration
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_auth_get(ql_wifi_ap_index_e idx, ql_wifi_ap_auth_s *auth);

/**
 * Set the WiFi configuration: Maximum number of stations allowed in station table.
 *
 * @param [in]   Index of AP mode
 * @param [in]   Maximum number of stations
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_max_sta_set(ql_wifi_ap_index_e idx, int max_sta_num);

/**
 * Get the WiFi configuration: Maximum number of stations allowed in station table.
 *
 * @param [in]    Index of AP mode
 * @param [out]   Maximum number of stations
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_ap_max_sta_get(ql_wifi_ap_index_e idx, int *max_sta_num);

/**
 * Get the WiFi configuration: the station information.
 *
 * @param [in]    Index of AP mode
 * @param [out]   station information
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */

extern int ql_wifi_ap_sta_info_get(ql_wifi_ap_index_e idx, ql_wifi_ap_sta_info_s *sta_info);


/**
 * Start the hostapd configuration after the appropriate hostapd.conf is modified.
 *
 * @param [in]    Index of AP mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 *
 * @dependencies
 *   ql_wifi_enable must be enable.   
 *
 */
extern int ql_wifi_ap_start(ql_wifi_ap_index_e idx);

/**
 * Stop the hostapd configuration.
 *
 * @param [in]    Index of AP mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   ql_wifi_enable must be enable.
 *
 */
extern int ql_wifi_ap_stop(ql_wifi_ap_index_e idx);

/**
 * Restart the hostapd configuration after the appropriate hostapd.conf is modified.
 *
 * @param [in]    Index of AP mode
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   ql_wifi_enable must be enable.
 *
 */
extern int ql_wifi_ap_restart(ql_wifi_ap_index_e idx);

/**
 * Set the WiFi configuration: Connect the hotspot ssid name.
 *
 * @param [in]   Index of AP mode
 * @param [in]   Connect hotspot name
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_sta_ssid_set(char *ssid);

/**
 * Get the WiFi configuration: Connect the hotspot ssid name.
 *
 * @param [in]    Index of AP mode
 * @param [out]   Connect hotspot name
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_sta_ssid_get(char *ssid);

/**
 * Set the WiFi configuration: Connect the hotspot authentication configuration.
 *
 * @param [in]   Index of AP mode
 * @param [in]   authentication configuration
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_sta_auth_set(ql_wifi_sta_auth_s *auth);

/**
 * Get the WiFi configuration: Connect the hotspot authentication configuration.
 *
 * @param [in]    Index of AP mode
 * @param [out]   authentication configuration
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_wifi_sta_auth_get(ql_wifi_sta_auth_s *auth);

/**
 * Activates the WPA supplicant configuration after wpa_supplicant.conf is modified.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 *
 * @dependencies
 *   ql_wifi_enable must be enable.
 * 
 */
extern int ql_wifi_sta_connect(void);

/**
 * Stop the WPA supplicant configuration.
 *
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 *
 * @dependencies
 *   ql_wifi_enable must be enable.
 * 
 */
extern int ql_wifi_sta_disconnect(void);

/**
 * Gets the status of the WiFi station mode connection.
 *
 * @param [out]      The WiFi station status
 *
 * @return 
 *   QL_WIFI_STATION_DISABLE(0)         WiFi station is not enable
 *   QL_WIFI_STATION_CONNECTED(1)       WiFi station is connected hotspot
 *   QL_WIFI_STATION_DISCONNECTED(2)    WiFi station is disconnect hotspot
 *
 * 
 */
extern int ql_wifi_sta_status(ql_wifi_station_status_e *status);


#endif /* end of __QL_WIFI_H__ */

