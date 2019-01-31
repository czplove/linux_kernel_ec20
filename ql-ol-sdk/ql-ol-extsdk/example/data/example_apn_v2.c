/**
 * @file example_apn_v2.c
 * @brief Quectel Openlinux APN examples.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ql_wwan_v2.h"

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

static void apn_set(void)
{
	char *sptr = NULL;
	ql_apn_info_s apn;

	memset(&apn, 0, sizeof(apn));
	sptr = input_fgets("\nPlease input profile index(1~24): ");
	if(!sptr || atoi(sptr) < 0 || atoi(sptr) > 24) {
		printf("\nInvalid profile index");
		return;
	}
	apn.profile_idx = atoi(sptr);

	sptr = input_fgets("\nPlease input PDP type(v4/v6/v4v6): ");
	if(sptr) {
		if(0 == strcmp(sptr, "v4")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV4;
		} else if(0 == strcmp(sptr, "v6")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV6;
		} else if(0 == strcmp(sptr, "v4v6")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV4V6;
		} else {
			printf("\nInvalid PDP type");
			return;
		}
	}

	sptr = input_fgets("\nPlease input apn name: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.apn_name, sptr);
	}
	sptr = input_fgets("\nPlease input username name: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.username, sptr);
	}
	sptr = input_fgets("\nPlease input passowrd: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.password, sptr);
	}
	if(QL_APN_Set(&apn)) {
		printf("\nSet apn failed");
	}
}

static void apn_get(void)
{
	char *sptr = NULL;
	ql_apn_info_s apn;

	memset(&apn, 0, sizeof(apn));
	sptr = input_fgets("\nPlease input profile index(1~24): ");
	if(!sptr || atoi(sptr) < 0 || atoi(sptr) > 24) {
		printf("\nInvalid profile index");
		return;
	}

	if(QL_APN_Get(atoi(sptr), &apn)) {
		printf("\nGet apn failed");
		return;
	}
	printf("\nprofile index: %d", apn.profile_idx);
	printf("\nPDP type: ");
	switch(apn.pdp_type) {
		case QL_APN_PDP_TYPE_IPV4:
			printf("IPv4");
			break;
		case QL_APN_PDP_TYPE_IPV6:
			printf("IPv6");
			break;
		case QL_APN_PDP_TYPE_IPV4V6:
			printf("IPv4 and IPv6");
			break;
		case QL_APN_PDP_TYPE_PPP:
			printf("PPP");
			break;
		default:
			printf("invalid");
			break;
	}
	printf("\nAuth protocol: ");
	switch(apn.auth_proto) {
		case QL_APN_AUTH_PROTO_DEFAULT:
			printf("Default");
			break;
		case QL_APN_AUTH_PROTO_NONE:
			printf("NONE");
			break;
		case QL_APN_AUTH_PROTO_PAP:
			printf("PAP");
			break;
		case QL_APN_AUTH_PROTO_CHAP:
			printf("CHAP");
			break;
		case QL_APN_AUTH_PROTO_PAP_CHAP:
			printf("PAP and CHAP");
			break;
		default:
			printf("invalid");
			break;
	}
	printf("\napn name: %s", apn.apn_name);
	printf("\nuser name: %s", apn.username);
	printf("\npassword: %s", apn.password);
}

void apn_add(void)
{
	char *sptr = NULL;
	ql_apn_add_s apn;
	unsigned char profile_index = 0;

	memset(&apn, 0, sizeof(apn));
	sptr = input_fgets("\nPlease input PDP type(v4/v6/v4v6): ");
	if(sptr) {
		if(0 == strcmp(sptr, "v4")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV4;
		} else if(0 == strcmp(sptr, "v6")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV6;
		} else if(0 == strcmp(sptr, "v4v6")) {
			apn.pdp_type = QL_APN_PDP_TYPE_IPV4V6;
		} else {
			printf("\nInvalid PDP type");
			return;
		}
	}

	sptr = input_fgets("\nPlease input apn name: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.apn_name, sptr);
	}
	sptr = input_fgets("\nPlease input username name: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.username, sptr);
	}
	sptr = input_fgets("\nPlease input passowrd: ");
	if(strlen(sptr) > 0) {
		strcpy(apn.password, sptr);
	}
	
	if(QL_APN_Add(&apn, &profile_index)) {
		printf("\nAdd apn failed");
	} else {
		printf("\nAdd profile index %d success", profile_index);
	}
}

void apn_del(void)
{
	char *sptr = NULL;
	unsigned char profile_index = 0;

	sptr = input_fgets("\nPlease input profile index(1~24): ");
	if(!sptr || atoi(sptr) < 0 || atoi(sptr) > 24) {
		printf("\nInvalid profile index");
		return;
	}
	profile_index = atoi(sptr);
	if(QL_APN_Del(profile_index)) {
		printf("\nDel apn failed");
	}
}

void apn_get_list(void)
{
	ql_apn_info_list_s apn_list;
	int i = 0;

	memset(&apn_list, 0, sizeof(apn_list));
	if(QL_APN_Get_Lists(&apn_list) < 0) {
		printf("\nGet APN list failure");
		return;
	}
	for(i = 0; i < apn_list.cnt; i++) {
		printf("\nprofile index: %d", apn_list.apn[i].profile_idx);
		printf("\nPDP type: ");
		switch(apn_list.apn[i].pdp_type) {
			case QL_APN_PDP_TYPE_IPV4:
				printf("IPv4");
				break;
			case QL_APN_PDP_TYPE_IPV6:
				printf("IPv6");
				break;
			case QL_APN_PDP_TYPE_IPV4V6:
				printf("IPv4 and IPv6");
				break;
			case QL_APN_PDP_TYPE_PPP:
				printf("PPP");
				break;
			default:
				printf("invalid");		
		}
		printf("\nAuth protocol: ");
		switch(apn_list.apn[i].auth_proto) {
			case QL_APN_AUTH_PROTO_DEFAULT:
				printf("Default");
				break;
			case QL_APN_AUTH_PROTO_NONE:
				printf("NONE");
				break;
			case QL_APN_AUTH_PROTO_PAP:
				printf("PAP");
				break;
			case QL_APN_AUTH_PROTO_CHAP:
				printf("CHAP");
				break;
			case QL_APN_AUTH_PROTO_PAP_CHAP:
				printf("PAP and CHAP");
				break;
			default:
				printf("invalid");
				break;
		}
		printf("\napn name: %s", apn_list.apn[i].apn_name);
		printf("\nuser name: %s", apn_list.apn[i].username);
		printf("\npassword: %s", apn_list.apn[i].password);
	}
}

int main(int argc, char **argv)
{
	char *sptr = NULL;

	while(1) {
		printf("\nCase 1: Set APN");
		printf("\nCase 2: Get APN");
		printf("\nCase 3: Add APN");
		printf("\nCase 4: Del APN");
		printf("\nCase 5: Get APN List");
		printf("\nCase 100: exit");

		sptr = input_fgets("\nPlease input case: ");
		switch(atoi(sptr)) {
			case 1:
				apn_set();
				break;
			case 2:
				apn_get();
				break;
			case 3:
				apn_add();
				break;
			case 4:
				apn_del();
				break;
			case 5:
				apn_get_list();
				break;
			case 100:
				return 0;
			default:
				printf("\nInvalid Case");
				break;
		}
	}
	return 0;
}
