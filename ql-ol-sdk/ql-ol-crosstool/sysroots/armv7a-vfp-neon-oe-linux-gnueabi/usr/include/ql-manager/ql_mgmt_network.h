/**
 * @file ql_mgtm_network.h
 * @brief Quectel Network interface function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_MGMT_NETWORK_H__
#define __QL_MGMT_NETWORK_H__

typedef enum {
	QL_MGMT_CLIENT_COMMAND_NETWORK_DMZ_ADD            = 0x0001,
	QL_MGMT_CLIENT_COMMAND_NETWORK_DMZ_DEL            = 0x0002,
	QL_MGMT_CLIENT_COMMAND_NETWORK_DMZ_GET            = 0x0003,
	QL_MGMT_CLIENT_COMMAND_NETWORK_SGMII_ENABLE       = 0x0004,
	QL_MGMT_CLIENT_COMMAND_NETWORK_SGMII_CONFIG       = 0x0005,
	QL_MGMT_CLIENT_COMMAND_NETWORK_SGMII_INFO         = 0x0006,
	QL_MGMT_CLIENT_COMMAND_NETWORK_LAN_CONFIG       =0x0007 
} ql_mgmt_client_command_network_e;

typedef enum {	
	QL_NETWORK_SGMII_SPEED_10 = 0,
	QL_NETWORK_SGMII_SPEED_100,
	QL_NETWORK_SGMII_SPEED_1000
} ql_network_sgmii_speed_e;

typedef enum {
	QL_NETWORK_SGMII_DUPLEX_HALF = 0,
	QL_NETWORK_SGMII_DUPLEX_FULL
} ql_network_sgmii_duplex_e;

struct network_dmz {
	unsigned int addr4;
};

struct network_sgmii_enable {
	bool state;
};

struct network_sgmii_config {
	/*
	 * if autoneg set true, the speed and duplex do not set.
	 * if set speed and duplex, the autoneg must be set false
	 */
	bool autoneg;
	char speed;
	char duplex;
};

struct network_sgmii_info {
	bool state;
	bool running;
	char speed;
	char duplex;
	bool autoneg;
	unsigned long long rx_pkts;
	unsigned long long rx_bytes;
	unsigned long long tx_pkts;
	unsigned long long tx_bytes;
}; 

struct network_lan_config {
	unsigned int gw_ip;
	unsigned int netmask;
	unsigned char enable_dhcp;
	unsigned int  dhcp_start_ip;
	unsigned int  dhcp_end_ip;
	unsigned int  lease_time;
};

typedef struct {
	union {
		struct network_dmz dmz;
		struct network_sgmii_enable sgmii_enable;
		struct network_sgmii_config sgmii_config;
		struct network_sgmii_info sgmii_info;
		struct network_lan_config lan_config;
	};
} ql_mgmt_client_network_s;

#endif /* end of __QL_MGMT_NETWORK_H__ */
