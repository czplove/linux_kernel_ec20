/**
 * @file ql_mgmt_data_call.h
 * @brief Quectel Linux Data_Call interface function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_MGMT_DATA_CALL_H__
#define __QL_MGMT_DATA_CALL_H__

typedef enum {
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_UP                  = 0x0001,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_DOWN                = 0x0002,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_INFO                = 0x0003,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_APN                 = 0x0004,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_APN_PROFILE_LIST    = 0x0005,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_APN_DEL             = 0x0006,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_APN_ADD             = 0x0007,
	QL_MGMT_CLIENT_COMMAND_DATA_CALL_DEFAULT_PROFILE     = 0x0008,
} ql_mgmt_client_command_data_call_e;

typedef enum {
	QL_MGMT_CLIENT_IND_DATA_CALL_STATE                   = 0x0001,
} ql_mgmt_client_ind_data_call_e;

#define QL_DATA_CALL_PROFILE_INDEX_MAX 24

typedef enum {
	QL_MGMT_CLIENT_DATA_CALL_STATE_DISCONNECTED = 0,
	QL_MGMT_CLIENT_DATA_CALL_STATE_CONNECTED,
} ql_mgmt_client_data_call_state_e;

struct ql_mgmt_client_data_call_pkt_statis {
	unsigned long pkts_tx;
	unsigned long pkts_rx;
	long long bytes_tx;
	long long bytes_rx;
	unsigned long pkts_dropped_tx;
	unsigned long pkts_dropped_rx;
};

struct ql_mgmt_client_data_call_v4_status {
	struct in_addr ip;
	struct in_addr gateway;
	struct in_addr pri_dns;
	struct in_addr sec_dns;
};

struct ql_mgmt_client_data_call_v6_status {
	struct in6_addr ip;
	struct in6_addr gateway;
	struct in6_addr pri_dns;
	struct in6_addr sec_dns;
};

struct ql_mgmt_client_data_call_info_v4 {
	char name[16];
	ql_mgmt_client_data_call_state_e state;
	bool reconnect;
	struct ql_mgmt_client_data_call_v4_status status;
	struct ql_mgmt_client_data_call_pkt_statis stats;
};

struct ql_mgmt_client_data_call_info_v6 {
	char name[16];
	ql_mgmt_client_data_call_state_e state;
	bool reconnect;
	struct ql_mgmt_client_data_call_v6_status status;
	struct ql_mgmt_client_data_call_pkt_statis stats;
};

struct ql_mgmt_client_data_call_up {
	unsigned char index;
	data_call_ip_family_e ip_family;
	bool reconnect;
	char apn_name[151];
	char username[128];
	char password[128];
};

struct ql_mgmt_client_data_call_down {
	unsigned char index;
	data_call_ip_family_e ip_family;
};

struct ql_mgmt_client_data_call_info {
	unsigned char index;
	data_call_ip_family_e ip_family;
	struct ql_mgmt_client_data_call_info_v4 v4;
	struct ql_mgmt_client_data_call_info_v6 v6;
};

struct ql_mgmt_client_data_call_ind_state {
	unsigned char index;
	char name[16];
	data_call_ip_family_e ip_family;
	ql_mgmt_client_data_call_state_e state;
	int reason_code;
	union {
		struct ql_mgmt_client_data_call_v4_status v4_status;
		struct ql_mgmt_client_data_call_v6_status v6_status;
	};
};

struct ql_mgmt_client_data_call_apn {
	unsigned char profile_type;
	unsigned char profile_index;
	unsigned char pdp_type;
	unsigned char auth_protocol;
	char apn_name[151];
	char username[128];
	char passwd[128];
};

struct ql_mgmt_client_data_call_apn_list {
	unsigned char profile_type;
	unsigned char len;
	char list[256];
};

struct ql_mgmt_client_data_call_default_profile {
	unsigned char qcmap_default_id;
        data_call_qcmap_tech_pref_e qcmap_tech_pref;
        data_call_ip_family_e ip_family;
};

typedef struct {
	union {
		struct ql_mgmt_client_data_call_up up;
		struct ql_mgmt_client_data_call_down down;
		struct ql_mgmt_client_data_call_info info;
		struct ql_mgmt_client_data_call_apn apn;
		struct ql_mgmt_client_data_call_apn_list apn_list;
		struct ql_mgmt_client_data_call_default_profile default_profile;
	};
} ql_mgmt_client_data_call_s;

#endif /* end of __QL_MGMT_DATA_CALL_H__ */

