/**
 * @file ap_mode.c
 * @brief Quectel Openlinux WiFi public function declarations.
 * 
 * @note this example setting QL_WIFI_WORK_MODE_AP0
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
	ql_wifi_ap_auth_s auth;

	ql_wifi_work_mode_set(QL_WIFI_WORK_MODE_AP0);
	ql_wifi_ap_ssid_set(QL_WIFI_AP_INDEX_AP0, "Quectel-WORK_MODE_AP0");
	ql_wifi_ap_mode_set(QL_WIFI_AP_INDEX_AP0, QL_WIFI_MODE_80211BGN);
	ql_wifi_ap_bandwidth_set(QL_WIFI_AP_INDEX_AP0, QL_WIFI_BANDWIDTH_HT20);
	ql_wifi_ap_channel_set(QL_WIFI_AP_INDEX_AP0, 11);

	auth.auth = QL_WIFI_AUTH_WPA_PSK;
	auth.wpa_psk.pairwise = QL_WIFI_AUTH_WPA_PAIRWISE_AUTO;
	auth.wpa_psk.group_rekey = 3600; /* one hour */
	strcpy(auth.wpa_psk.passwd, "12345678");
	ql_wifi_ap_auth_set(QL_WIFI_AP_INDEX_AP0, &auth);

	ql_wifi_enable();

	return 0;
}

