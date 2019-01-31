#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ql_wifi.h"

static inline char *input_fgets(char *msg, ...)
{
	static char ptr[128] = { 0 };
	va_list ap;

	va_start(ap, msg);
	vprintf(msg, ap);
	va_end(ap);
	memset(ptr, 0, sizeof(ptr));
	fgets(ptr, sizeof(ptr), stdin);
	if(strlen(ptr) > 0) {
		if('\n' == ptr[strlen(ptr) - 1]) {
			ptr[strlen(ptr) - 1] = '\0';
		}
	}
	return ptr;
}

static void wifi_event_callback(ql_wifi_event_s *event, void *arg)
{
	printf("\nEvent id: %d, status %d", event->id, event->status);
	switch(event->id) {
		case QL_WIFI_EVENT_ENABLE_STATUS:
			if(QL_WIFI_STATUS_ENABLE == event->status) {
				printf("\n Enable WiFi Success");
			} else {
				printf("\n Enable WiFi failure: error %d", event->status);
			}
			break;
		case QL_WIFI_EVENT_DISABLE_STATUS:
			if(QL_WIFI_STATUS_DISABLE == event->status) {
				printf("\n Disable WiFi Success");
			} else {
				printf("\n Disable WiFi failure: error %d", event->status);
			}
			break;
		default:
			printf("\n");
	}
	fflush(stdout);
}

static int wifi_ap_config(ql_wifi_ap_index_e idx)
{
	char *sptr = NULL;
	ql_wifi_ap_auth_s auth;
	
	/* setting SSID */
	sptr = input_fgets("\nPlease input WiFi index %d ssid(maximun size 32): ", idx);
	if(ql_wifi_ap_ssid_set(idx, sptr)) {
		printf("\nSet WiFi ssid failure");
		return -1;
	}
	/* setting IEEE 802.11 Mode */
	sptr = input_fgets("\nPlease input WiFi index %d IEEE 802.11 mode(0-b/1-bg/2-bgn/3-a/4-an/5-ac): ", idx);
	if(atoi(sptr) < QL_WIFI_MODE_80211B || atoi(sptr) > QL_WIFI_MODE_80211AC
	   || ql_wifi_ap_mode_set(idx, atoi(sptr))) {
		printf("\nSet IEEE 802.11 mode failure");
		return -1;
	}
	/* 802.11a/b/bg do not configure bandwidth */
	if(QL_WIFI_MODE_80211B != atoi(sptr) && QL_WIFI_MODE_80211A != atoi(sptr) && QL_WIFI_MODE_80211BG != atoi(sptr))
	{
		/* setting bandwidth */
		sptr = input_fgets("\nPlease input WiFi index %d Bandwidth(0-20MHz/1-40MHz/2-80MHz): ", idx);
		if(atoi(sptr) < QL_WIFI_BANDWIDTH_HT20 || atoi(sptr) > QL_WIFI_BANDWIDTH_HT80
		   || ql_wifi_ap_bandwidth_set(idx, atoi(sptr))) {
			printf("\nSet bandwidth failure");
			return -1;
		}
	}
	/* setting channel */
	sptr = input_fgets("\nPlease input WiFi index %d channel: ", idx);
	if(ql_wifi_ap_channel_set(idx, atoi(sptr))) {
		printf("\nSet channel failure");
		return -1;
	}
	/* setting max station number */
	sptr = input_fgets("\nPlease input WiFi index %d maximun station(1-16):", idx);
	if(atoi(sptr) <= 0 || atoi(sptr) > 16 || ql_wifi_ap_max_sta_set(idx, atoi(sptr))) {
		printf("\nSet maximun stations failure");
		return -1;
	}
	/* setting Auth */
	memset(&auth, 0, sizeof(ql_wifi_ap_auth_s));
	sptr = input_fgets("\nPlease input WiFi index %d type of "
			   "authentication(0-OPEN/1-WPA PSK/2-WPA2 PSK): ", idx);
	if(atoi(sptr) < QL_WIFI_AUTH_OPEN || atoi(sptr) > QL_WIFI_AUTH_WPA2_PSK) {
		printf("\nSet auth failure");
		return -1;
	} else {
		auth.auth = atoi(sptr);
		if(QL_WIFI_AUTH_OPEN != atoi(sptr)) {
			sptr = input_fgets("\nPlease input WiFi index %d WPA PSK pairwise(0-AUTH/1-TKIP/2-AES): ", idx);
			if(atoi(sptr) < QL_WIFI_AUTH_WPA_PAIRWISE_AUTO
			   || atoi(sptr) > QL_WIFI_AUTH_WPA_PAIRWISE_AES) {
				printf("\nSet WPA PSK pairwise failure");
				return -1;
			}
			auth.wpa_psk.pairwise = atoi(sptr);
			sptr = input_fgets("\nPlease input WiFi index %d WPA PSK password: ", idx);
			strcpy(auth.wpa_psk.passwd, sptr);
		}
		if(ql_wifi_ap_auth_set(idx, &auth)) {
			printf("\nSet auth failure");
			return -1;
		}
	}
	return 0;
}

static int wifi_sta_config(void)
{
	char *sptr = NULL;
	ql_wifi_sta_auth_s auth;

	sptr = input_fgets("\nPlease input STA ssid(maximun size 32): ");
	if(ql_wifi_sta_ssid_set(sptr)) {
		printf("\nSet STA ssid failure");
		return -1;
	}
	memset(&auth, 0, sizeof(ql_wifi_sta_auth_s));
	sptr = input_fgets("\nPlease input STA type of authentication(0-OPEN/1-WPA PSK/2-WPA2 PSK): ");
	if(atoi(sptr) < QL_WIFI_AUTH_OPEN || atoi(sptr) > QL_WIFI_AUTH_WPA2_PSK) {
		printf("\nSet STA auth failure");
		return -1;
	} else {
		auth.auth = atoi(sptr);
		if(QL_WIFI_AUTH_OPEN != atoi(sptr)) {
			sptr = input_fgets("\nPlease input STA WPA PSK pairwise(0-AUTH/1-TKIP/2-AES): ");
			if(atoi(sptr) < QL_WIFI_AUTH_WPA_PAIRWISE_AUTO
			   || atoi(sptr) > QL_WIFI_AUTH_WPA_PAIRWISE_AES) {
				printf("\nSet WPA PSK pairwise failure");
				return -1;
			}
			auth.wpa_psk.pairwise = atoi(sptr);
			sptr = input_fgets("\nPlease input STA WPA PSK password: ");
			strcpy(auth.wpa_psk.passwd, sptr);
		}
		if(ql_wifi_sta_auth_set(&auth)) {
			printf("\nSet STA auth failure");
			return -1;
		}
	}
	return 0;
}

static int wifi_ap_config_get(ql_wifi_ap_index_e idx)
{
	printf("\nThe WiFi AP index %d configuraction:", idx);

	/* get SSID */
	char ssid[32] = { 0 };
	if(ql_wifi_ap_ssid_get(idx, ssid)) {
		printf("\nget WiFi AP index %d SSID failure", idx);
		return -1;
	}
	printf("\n\t SSID: %s", ssid);

	/* get IEEE80211 Mode */
	ql_wifi_mode_type_e mode = QL_WIFI_MODE_80211B;
	if(ql_wifi_ap_mode_get(idx, &mode)) {
		printf("\nget WiFi AP index %d IEEE80211 Mode failure", idx);
		return -1;
	}
	switch(mode) {
		case QL_WIFI_MODE_80211B:
			printf("\n\t IEEE80211 Mode: 11b");
			break;
		case QL_WIFI_MODE_80211BG:
			printf("\n\t IEEE80211 Mode: 11bg");
			break;
		case QL_WIFI_MODE_80211BGN:
			printf("\n\t IEEE80211 Mode: 11bgn");
			break;			
		case QL_WIFI_MODE_80211A:
			printf("\n\t IEEE80211 Mode: 11a");
			break;			
		case QL_WIFI_MODE_80211AN:
			printf("\n\t IEEE80211 Mode: 11an");
			break;			
		case QL_WIFI_MODE_80211AC:
			printf("\n\t IEEE80211 Mode: 11ac");
			break;
		default:
			printf("\nget WiFi AP index %d invalid IEEE80211 Mode(%d)", idx, mode);
			return -1;
	}

	/* get Bandwidth */
	ql_wifi_bandwidth_type_e bandwidth = QL_WIFI_BANDWIDTH_HT20;
	if(ql_wifi_ap_bandwidth_get(idx, &bandwidth)) {
		printf("\nget WiFi AP index %d bandwidth failure", idx);
		return -1;
	}
	switch(bandwidth) {
		case QL_WIFI_BANDWIDTH_HT20:
			printf("\n\t Bandwidth: 20MHz");
			break;
		case QL_WIFI_BANDWIDTH_HT40:
			printf("\n\t Bandwidth: 40MHz");
			break;
		case QL_WIFI_BANDWIDTH_HT80:
			printf("\n\t Bandwidth: 80MHz");
			break;
		default:
			printf("\nget WiFi AP index %d invalid Bandwidth(%d)", idx, bandwidth);
			return -1;
	}

	/* get Channel */
	int channel = 0;
	if(ql_wifi_ap_channel_get(idx, &channel)) {
		printf("\nget WiFi AP index %d channel failure", idx);
		return -1;
	}
	printf("\n\t Channel: %d", channel);

	/* get Maximun stations */
	int max_sta = 0;
	if(ql_wifi_ap_max_sta_get(idx, &max_sta)) {
		printf("\nget WiFi AP index %d maximun stations failure", idx);
		return -1;
	}
	printf("\n\t Maximun station: %d", max_sta);

	/* get Auth */
	ql_wifi_ap_auth_s auth;
	memset(&auth, 0, sizeof(auth));
	if(ql_wifi_ap_auth_get(idx, &auth)) {
		printf("\nget WiFi AP index %d auth failure", idx);
		return -1;
	}
	switch(auth.auth) {
		case QL_WIFI_AUTH_OPEN:
			printf("\n\t Auth: OPEN");
			break;
		case QL_WIFI_AUTH_WPA_PSK:
			printf("\n\t Auth: WPA-PSK");
			break;
		case QL_WIFI_AUTH_WPA2_PSK:
			printf("\n\t Auth: WPA2-PSK");
			break;
		case QL_WIFI_AUTH_WPA_WPA2_PSK_BOTH:
			printf("\n\t Auth: WPA-PSK & WPA2-PSK");
			break;
		default:
			printf("\nget WiFi AP index %d invalid auth(%d)", idx, auth.auth);
			return -1;
	}
	if(QL_WIFI_AUTH_OPEN != auth.auth) {
		switch(auth.wpa_psk.pairwise) {
			case QL_WIFI_AUTH_WPA_PAIRWISE_AUTO:
				printf("\n\t   Pairwise: AUTO");
				break;
			case QL_WIFI_AUTH_WPA_PAIRWISE_AES:
				printf("\n\t   Pairwise: AES");
				break;
			case QL_WIFI_AUTH_WPA_PAIRWISE_TKIP:
				printf("\n\t   Pairwise: TKIP");
				break;
			default:
				printf("\n\t   get WiFi AP index %d invalid WPA PSK pairwise(%d)",
					idx, auth.wpa_psk.pairwise);
				return -1;
		}
		printf("\n\t   Password: %s", auth.wpa_psk.passwd);
		printf("\n\t   Group Rekey: %d", auth.wpa_psk.group_rekey);
	}

	/* get wifi station information */
	ql_wifi_ap_sta_info_s sta_info;
	memset(&sta_info, 0, sizeof(sta_info));
	if(ql_wifi_ap_sta_info_get(idx, &sta_info)) {
		printf("\nget WiFi AP index %d station information failure", idx);
		return -1;
	}
	printf("\nSta number: %d", sta_info.num);
	return 0;
}

static int wifi_sta_config_get(void)
{
	printf("\nThe WiFi STA configuraction:");

	/* get sta connect ssid */
	char ssid[32] = { 0 };
	if(ql_wifi_sta_ssid_get(ssid)) {
		printf("\nget WiFi STA connect ssid failure");
		return -1;
	}
	printf("\n\t SSID: %s", ssid);

	/* get sta auth */
	ql_wifi_sta_auth_s auth;
	memset(&auth, 0, sizeof(auth));
	if(ql_wifi_sta_auth_get(&auth)) {
		printf("\nget WiFi STA auth failure");
		return -1;
	}
	switch(auth.auth) {
		case QL_WIFI_AUTH_OPEN:
			printf("\n\t Auth: OPEN");
			break;
		case QL_WIFI_AUTH_WPA_PSK:
			printf("\n\t Auth: WPA-PSK");
			break;
		case QL_WIFI_AUTH_WPA2_PSK:
			printf("\n\t Auth: WPA2-PSK");
			break;
		case QL_WIFI_AUTH_WPA_WPA2_PSK_BOTH:
			printf("\n\t Auth: WPA-PSK & WPA2-PSK");
			break;
		default:
			printf("\nget WiFi STA invalid auth(%d)", auth.auth);
			return -1;
	}
	if(QL_WIFI_AUTH_OPEN != auth.auth) {
		switch(auth.wpa_psk.pairwise) {
			case QL_WIFI_AUTH_WPA_PAIRWISE_AUTO:
				printf("\n\t   Pairwise: AUTO");
				break;
			case QL_WIFI_AUTH_WPA_PAIRWISE_AES:
				printf("\n\t   Pairwise: AES");
				break;
			case QL_WIFI_AUTH_WPA_PAIRWISE_TKIP:
				printf("\n\t   Pairwise: TKIP");
				break;
			default:
				printf("\n\t   get WiFi STA invalid WPA PSK pairwise(%d)",
					auth.wpa_psk.pairwise);
				return -1;
		}
		printf("\n\t   Password: %s", auth.wpa_psk.passwd);
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *sptr = NULL;

	while(1) {
		printf("\ncase 1: Configure WiFi");
		printf("\ncase 2: Enable WiFi");
		printf("\ncase 3: Activate Hostapd");
		printf("\ncase 4: Activate WPA Supplicant");
		printf("\ncase 5: Get WPA Supplicant status");
		printf("\ncase 6: Get WiFi configuraction");
		printf("\ncase 7: Register Event Callback");
		printf("\ncase 8: Enable Module WiFi");
		printf("\ncase 9: Get WiFi status");
		printf("\ncase 100: exit");
		sptr = input_fgets("\nPlease input case: ");
		switch(atoi(sptr)) {
			case 1:
			{
				sptr = input_fgets("\nPlease input WiFi work mode(0-STA/1-AP0/2-AP0+STA/3-AP0+AP1): ");
				if(atoi(sptr) <  QL_WIFI_WORK_MODE_STA || atoi(sptr) > QL_WIFI_WORK_MODE_AP0_AP1
				   || ql_wifi_work_mode_set(atoi(sptr))) {
					printf("\nSet WiFi work mode failure");
					break;
				}
				if(QL_WIFI_WORK_MODE_STA == atoi(sptr)) {
					wifi_sta_config();
				} else if(QL_WIFI_WORK_MODE_AP0 == atoi(sptr)) {
					wifi_ap_config(QL_WIFI_AP_INDEX_AP0);
				} else if(QL_WIFI_WORK_MODE_AP0_STA == atoi(sptr)) {
					wifi_ap_config(QL_WIFI_AP_INDEX_AP0);
					wifi_sta_config();
				} else if(QL_WIFI_WORK_MODE_AP0_AP1 == atoi(sptr)) {
					wifi_ap_config(QL_WIFI_AP_INDEX_AP0);
					wifi_ap_config(QL_WIFI_AP_INDEX_AP1);
				}
				break;
			}
			case 2:
			{
				sptr = input_fgets("\nPlease input WiFi status(0-Disable/1-Enable"
					"/2-Async Enable/3-Async Disable): ");
				if(0 == atoi(sptr)) {
					ql_wifi_disable();
				} else if(1 == atoi(sptr)) {
					ql_wifi_enable();
				} else if(2 == atoi(sptr)) {
					ql_wifi_enable_async();
				} else if(3 == atoi(sptr)) {
					ql_wifi_disable_async();
				}else {
					printf("\nInvalid input WiFi status");
				}
				break;
			}
			case 3:
			{
				int idx;
				sptr = input_fgets("\nPlease input WiFi index(0-AP0/1-AP1): ");
				if(0 != atoi(sptr) && 1 != atoi(sptr)) {
					printf("\nInvalid WiFi index");
					break;
				}
				idx = atoi(sptr);
				sptr = input_fgets("\nPlease input WiFi hostapd status(0-Start/1-Stop/2-Restart): ");
				switch(atoi(sptr)) {
					case 0:
						ql_wifi_ap_start(idx);
						break;
					case 1:
						ql_wifi_ap_stop(idx);
						break;
					case 2:
						ql_wifi_ap_restart(idx);
						break;
					default:
						printf("\nInvalid hostapd status");
						break;
				}
				break;
			}
			case 4:
			{
				sptr = input_fgets("\nPlease input WiFi supplicat status(0-connect/1-disconnect): ");
				switch(atoi(sptr)) {
					case 0:
						ql_wifi_sta_connect();
						break;
					case 1:
						ql_wifi_sta_disconnect();
						break;
					default:
						printf("\nInvalid WPA supplicant status");
						break;
				}
				break;	
			}
			case 5:
			{
				ql_wifi_station_status_e status;

				if(ql_wifi_sta_status(&status)) {
					printf("\nGet WPA Supplicant status failure");
					break;
				}
				if(QL_WIFI_STATION_DISABLE == status) {
					printf("\nWiFi Station Disable");
				} else if(QL_WIFI_STATION_CONNECTED == status) {
					printf("\nWiFi Station connect hotspot");
				} else {
					printf("\nWiFi Station disconnect hotspot");
				}
				break;
			}
			case 6:
			{
				ql_wifi_work_mode_e work_mode = QL_WIFI_WORK_MODE_AP0;
				if(ql_wifi_work_mode_get(&work_mode)) {
					printf("\nGet WiFi Work mode failure");
					break;
				}
				switch(work_mode) {
					case QL_WIFI_WORK_MODE_STA:
						printf("\nWiFi Work mode: Only STA");
						break;
					case QL_WIFI_WORK_MODE_AP0:
						printf("\nWiFi Work mode: Only AP0");
						break;
					case QL_WIFI_WORK_MODE_AP0_STA:
						printf("\nWiFi Work mode: AP0 + STA");
						break;
					case QL_WIFI_WORK_MODE_AP0_AP1:
						printf("\nWiFi Work mode: AP0 + AP1");
						break;
					default:
						printf("\nInvalid Work Mode(%d)", work_mode);
						break;
				}
				wifi_ap_config_get(QL_WIFI_AP_INDEX_AP0);
				wifi_ap_config_get(QL_WIFI_AP_INDEX_AP1);
				wifi_sta_config_get();
				break;
			}
			case 7:
			{
				sptr = input_fgets("\nPlease input WiFi handle register(0-register/1-unregister): ");
				switch(atoi(sptr)) {
					case 0:
						ql_wifi_register_handle(wifi_event_callback, NULL);
						break;
					case 1:
						ql_wifi_unregister_handle();
						break;
					default:
						printf("\nInvalid WiFi handle register");
						break;
				}
				break;
			}
			case 8:
			{
				sptr = input_fgets("\nPlease input WiFi module status(1-Enable/0-Disable): ");
				if(0 == atoi(sptr)) {
					ql_wifi_module_disable();
				} else if(1 == atoi(sptr)) {
					ql_wifi_module_enable();
				} else {
					printf("\nInvalid input WiFi status");
				}
				break;
			}
			case 9:
			{
				ql_wifi_status_e status = QL_WIFI_STATUS_DISABLE;
				if(ql_wifi_status_get(&status)) {
					printf("\nGet WiFi status failure");
					break;
				}
				printf("get WiFi status %d", status);
				break;
			}
			case 100:
				return 0;
			default:
				printf("\nInvalid Case");
				break;
		}
	}
	return 0;
}
