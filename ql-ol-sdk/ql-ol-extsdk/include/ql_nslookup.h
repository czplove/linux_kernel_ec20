/**
 *@file     quectel_nslookup.h
 *@date     2017-07-05
 *@author   running.qian
 *@brief    Simple DNS Reslove
 */
#ifndef _QUECTEL_NSLOOKUP_H_
#define _QUECTEL_NSLOOKUP_H_

#include <arpa/inet.h>

typedef enum  {
    QUERY_IPV4_E,
    QUERY_IPV6_E
} QUERY_IP_TYPE;


typedef struct {
    int addr_cnt;//must be set 0 when call QL_nslookup 
    struct in_addr addr[5];
    
    int addr6_cnt;//must be set 0 when call QL_nslookup
    struct in6_addr addr6[5];
} hostaddr_info_u;

/**
 * Perform a DNS query by sending a packet
 */
void QL_nslookup(char *host, char *dns_server_ip, QUERY_IP_TYPE ip_type, hostaddr_info_u *resolved_addr);
#endif