/**
 * @file sta_mode.c
 * @brief Quectel Openlinux WiFi public function declarations.
 * 
 * @note this example setting QL_WIFI_WORK_MODE_STA
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
	ql_wifi_sta_auth_s auth;

	ql_wifi_work_mode_set(QL_WIFI_WORK_MODE_STA);
	ql_wifi_sta_ssid_set("Quectel-test");
	auth.auth = QL_WIFI_AUTH_WPA_PSK;
	auth.wpa_psk.pairwise = QL_WIFI_AUTH_WPA_PAIRWISE_AUTO;
	strcpy(auth.wpa_psk.passwd, "12345678");
	ql_wifi_sta_auth_set(&auth);

	ql_wifi_enable();
	return 0;
}

