/**
 * @file ap_sta_mode.c
 * @brief Quectel Openlinux WiFi public function declarations.
 * 
 * @note this example setting QL_WIFI_WORK_MODE_AP0_STA
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
 
#include "ql_wifi.h"


 int main(int argc, char **argv)
 {
	ql_wifi_ap_auth_s ap_auth;
	ql_wifi_sta_auth_s sta_auth;

	/* setting AP0 */
	ql_wifi_work_mode_set(QL_WIFI_WORK_MODE_AP0_STA);
	ql_wifi_ap_ssid_set(QL_WIFI_AP_INDEX_AP0, "Quectel-WORK_MODE_AP0");
	ql_wifi_ap_mode_set(QL_WIFI_AP_INDEX_AP0, QL_WIFI_MODE_80211BGN);
	ql_wifi_ap_bandwidth_set(QL_WIFI_AP_INDEX_AP0, QL_WIFI_BANDWIDTH_HT20);
	ql_wifi_ap_channel_set(QL_WIFI_AP_INDEX_AP0, 11);

	ap_auth.auth = QL_WIFI_AUTH_WPA_PSK;
	ap_auth.wpa_psk.pairwise = QL_WIFI_AUTH_WPA_PAIRWISE_AUTO;
	ap_auth.wpa_psk.group_rekey = 3600; /* one hour */
	strcpy(ap_auth.wpa_psk.passwd, "12345678");
	ql_wifi_ap_auth_set(QL_WIFI_AP_INDEX_AP0, &ap_auth);


	/* setting STA */
	ql_wifi_sta_ssid_set("Quectel-test");

	sta_auth.auth = QL_WIFI_AUTH_WPA_PSK;
	sta_auth.wpa_psk.pairwise = QL_WIFI_AUTH_WPA_PAIRWISE_AUTO;
	strcpy(sta_auth.wpa_psk.passwd, "12345678");
	ql_wifi_sta_auth_set(&sta_auth);

	ql_wifi_enable();
	return 0;
 }

