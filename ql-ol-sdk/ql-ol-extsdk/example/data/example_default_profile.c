/**
 * @file example_default_profile.c
 * @brief Quectel Openlinux setting qcmap default profile examples.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
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

void set_default_profile(void)
{
	
	char *sptr;
	ql_data_call_default_profile_s profile;

	sptr = input_fgets("\nPlease input default profile index(1~24): ");
	if(!sptr || atoi(sptr) < 0 || atoi(sptr) > 24) {
		printf("\nInvalid profile index");
		return;
	}
	profile.profile_idx = atoi(sptr);
	if(QL_Data_Call_Set_Default_Profile(&profile)) {
		printf("\nset default profile failure");
	}
}

void get_default_profile(void)
{
	ql_data_call_default_profile_s profile;

	memset(&profile, 0, sizeof(profile));
	if(QL_Data_Call_Get_Default_Profile(&profile)) {
		printf("\nGet default profile failure");
		return;
	}
	printf("\nDefault Profile:");
	printf("\n    Profile index: %d", profile.profile_idx);
}

int main(int argc, char **argv)
{
	char *sptr = NULL;

	while(1) {
		printf("\nCase 1: Set Default Profile");
		printf("\nCase 2: Get Default Profile");
		printf("\nCase 100: exit");

		sptr = input_fgets("\nPlease input case: ");
		switch(atoi(sptr)) {
			case 1:
				set_default_profile();
				break;
			case 2:
				get_default_profile();
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
