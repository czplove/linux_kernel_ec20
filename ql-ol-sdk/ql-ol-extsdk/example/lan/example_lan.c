#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ql_network.h"

static int quectel_ipv4_check(char *addr4)
{
	int ip[4] = { 0 };
	int i = 0;
	char s[64] = { 0 };
	char *ptr;

	if(!addr4 || strlen(addr4) < 7) {
		return -1;
	}

	ptr = addr4;
	while('\0' != *ptr) {
		if('.' != *ptr && (*ptr < 48 || *ptr > 57)) {
			return -1;
		}
		ptr++;
	}
	
	if(4 != sscanf(addr4, "%d.%d.%d.%d%s", &ip[0], &ip[1], &ip[2], &ip[3], s)){
		return -1;
	}
	
	if(0 != s[0]) {
		return -1;
	}

	for(i = 0; i < sizeof(ip)/sizeof(ip[0]); i++) {
		if(ip[i] < 0 || ip[i] > 255) {
			return -1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	char sptr[120];
	int enable_dhcp;
	int lease_time;
	ql_lan_dhcp_config_s lan_config;
	memset(&lan_config, 0, sizeof(lan_config));
	
	while(1) {
		printf("\ncase 1: Set LAN Config");
		printf("\ncase 2: Get LAN Config");
		printf("\ncase 3: Exit");
		printf("Please select case:\n");
		scanf("%s",sptr);
		switch(atoi(sptr)) {
			case 1:
			{
				while(1) {
					printf("Please Input Gateway IP, such as 192.168.30.1\n");
					scanf("%s",sptr);
					if(-1 != quectel_ipv4_check(sptr)) {
						strcpy(lan_config.gw_ip, sptr);
						break;
					}										
					printf("%s : error format  \n\n", sptr );
				}
				while(1) {
					printf("Please Input Subnet Mask: such as 255.255.255.0\n");
					scanf("%s",sptr);
					if(-1 != quectel_ipv4_check(sptr)) {
						strcpy(lan_config.netmask, sptr);
						break;
					}
					printf("%s : error format  \n\n", sptr );
				}
				while(1) {									
					printf("Please Input DHCP Start IP: such as 192.168.30.1\n");
					scanf("%s",sptr);
					if(-1 != quectel_ipv4_check(sptr)) {
						strcpy(lan_config.dhcp_start_ip, sptr);
						break;
					}
					printf("%s : error format  \n\n", sptr );
				}
				while(1) {
					printf("Please Input DHCP End IP: such as 192.168.30.254\n");
					scanf("%s",sptr);
					if(-1 != quectel_ipv4_check(sptr)) {
						strcpy(lan_config.dhcp_end_ip, sptr);
						break;
					}
					printf("%s : error format  \n\n", sptr );
				}
				while(1) {
					printf("Please Input DHCP enable value, such as 1 or 0\n");
					scanf("%d",&enable_dhcp);
					if( 1 == enable_dhcp || 0 == enable_dhcp) {
						lan_config.enable_dhcp = enable_dhcp;
						break;
					}
					printf("%d : error format  \n\n", enable_dhcp );
				}
				while(1) {				
					printf("Please Input DHCP lease time, in seconds\n");
					scanf("%d",&lease_time);
					if(0 < lease_time && 2147483647 > lease_time) {
						lan_config.lease_time = lease_time;
						break;
					}
					printf("%d : error format  \n\n", lease_time );
				}
				
				if(QL_LAN_DHCP_Config_Set(lan_config) != -1) {
					printf("Set LAN Config Success!\n");
				}else {
					printf("set LAN Config Failure!\n");
				}				
				break;
			}
			case 2:
			{
				ql_lan_dhcp_config_s  lan_config;
				memset(&lan_config, 0, sizeof(lan_config));
				if(QL_LAN_DHCP_Config_Get(&lan_config) != -1) {
					printf("Gateway IP :%s\n", lan_config.gw_ip);
					printf("Subnet Mask :%s\n", lan_config.netmask);
					printf("DHCP Start IP :%s\n", lan_config.dhcp_start_ip);
					printf("DHCP End IP :%s\n", lan_config.dhcp_end_ip);
					printf("LAN Enable :%d\n", lan_config.enable_dhcp);
					printf("DHCP lease time :%d\n", lan_config.lease_time);
				}else {
					printf("Get LAN Failure!\n");
				}
				break;
			}
			case 3:
			{
				return 0;
			}
			default:
				printf("\nInvalid Case");
				break;
		}
	}
	return 0;
}

