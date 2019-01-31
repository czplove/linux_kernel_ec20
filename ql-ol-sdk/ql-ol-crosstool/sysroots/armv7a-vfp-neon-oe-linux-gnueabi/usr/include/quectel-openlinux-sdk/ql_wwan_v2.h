/**
 * @file ql_wwan_v2.h
 * @brief Quectel WWAN data call lib public function declarations.
 * 
 * @note version 2
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_WWAN_V2_H__
#define __QL_WWAN_V2_H__

#include <stdbool.h>
#include <netinet/in.h>

typedef enum {
	QL_DATA_CALL_ERROR_NONE = 0,
	QL_DATA_CALL_ERROR_INVALID_PARAMS,
} ql_data_call_error_e;

typedef enum {
	QL_DATA_CALL_DISCONNECTED = 0,            /*!< call is disconnected */
	QL_DATA_CALL_CONNECTED,                   /*!< call is connected */
} ql_data_call_state_e;

typedef enum {
	QL_DATA_CALL_TYPE_IPV4 = 0,           /*!< IPv4 call. */
	QL_DATA_CALL_TYPE_IPV6,               /*!< IPv6 call. */
	QL_DATA_CALL_TYPE_IPV4V6,             /*!< IPv4 and IPv6 call (Only used call start or stop). */
} ql_data_call_ip_family_e;

typedef enum {
	QL_APN_PDP_TYPE_IPV4 = 0,
	QL_APN_PDP_TYPE_PPP,
	QL_APN_PDP_TYPE_IPV6,
	QL_APN_PDP_TYPE_IPV4V6,
} ql_apn_pdp_type_e;

typedef enum {
	QL_APN_AUTH_PROTO_DEFAULT = 0,
	QL_APN_AUTH_PROTO_NONE,
	QL_APN_AUTH_PROTO_PAP,
	QL_APN_AUTH_PROTO_CHAP,
	QL_APN_AUTH_PROTO_PAP_CHAP,
} ql_apn_auth_proto_e;

#define QL_APN_MAX_LIST 8
#define QL_APN_NAME_SIZE 150
#define QL_APN_USERNAME_SIZE 127
#define QL_APN_PASSWORD_SIZE 127

struct v4_address_status {
	struct in_addr ip;                    /*!< Public IPv4 address. */
	struct in_addr gateway;               /*!< Public IPv4 gateway. */
	struct in_addr pri_dns;               /*!< Primary Domain Name Service IP address. */
	struct in_addr sec_dns;               /*!< Secondary Domain Name Service IP address. */
};

struct v6_address_status {
	struct in6_addr ip;                   /*!< Public IPv6 address. */
	struct in6_addr gateway;              /*!< Public IPv6 gateway. */
	struct in6_addr pri_dns;              /*!< Primary Domain Name Service IPv6 address. */
	struct in6_addr sec_dns;              /*!< Secondary Domain Name Service IPv6 address. */
};

typedef struct {
	char profile_idx;                               /*!< UMTS/CMDA profile ID. */
	char name[16];                                  /*!< Interface Name. */
	ql_data_call_ip_family_e ip_family;             /*!< IP version. */
	ql_data_call_state_e state;                     /*!< The dial status. */
	ql_data_call_error_e err;                       /*!< The Reason code after data call disconnected. */
	union {
		struct v4_address_status v4;            /*!< IPv4 information. */
		struct v6_address_status v6;            /*!< IPv6 information. */
	};
} ql_data_call_state_s;

/*
 *!< Client callback function used to post event indications.
 */
typedef void (*ql_data_call_evt_cb_t)(ql_data_call_state_s *state);

typedef struct {
	char profile_idx;                        /*!< UMTS/CMDA profile ID. */
	bool reconnect;                          /*!< Whether to re-dial after disconnecting the network. */
	ql_data_call_ip_family_e ip_family;      /*!< IP version. */
	char cdma_username[QL_APN_USERNAME_SIZE];     /*!< Username used during data network authentication. */
	char cdma_password[QL_APN_PASSWORD_SIZE];     /*!< Password to be used during data network authentication. */
} ql_data_call_s;

struct pkt_stats {
	unsigned long pkts_tx;              /*!< Number of packets transmitted. */
	unsigned long pkts_rx;              /*!< Number of packets received. */
	long long bytes_tx;                 /*!< Number of bytes transmitted. */
	long long bytes_rx;                 /*!< Number of bytes received. */
	unsigned long pkts_dropped_tx;      /*!< Number of transmit packets dropped. */
	unsigned long pkts_dropped_rx;      /*!< Number of receive packets dropped. */
};

struct v4_info {
	char name[16];                          /*!< Interface Name. */
	ql_data_call_state_e state;             /*!< The dial status. */
	bool reconnect;                         /*!< re-dial flag. */
	struct v4_address_status addr;          /*!< IPv4 IP Address information. */
	struct pkt_stats stats;                 /*!< IPv4 statics */
};

struct v6_info {
	char name[16];                          /*!< Interface Name. */
	ql_data_call_state_e state;             /*!< The dial status. */
	bool reconnect;                         /*!< re-dial flag. */
	struct v6_address_status addr;          /*!< IPv6 IP Address information. */
	struct pkt_stats stats;                 /*!< IPv6 statics */
};

typedef struct {
	char profile_idx;                        /*!< UMTS/CDMA profile ID. */
	ql_data_call_ip_family_e ip_family;      /*!< IP version. */
	struct v4_info v4;                       /*!< IPv4 information */
	struct v6_info v6;                       /*!< IPv6 information */
} ql_data_call_info_s;

typedef struct {
	char profile_idx;
} ql_data_call_default_profile_s;

typedef struct {
	unsigned char profile_idx;              /*!< UMTS/CDMA profile ID. */
	ql_apn_pdp_type_e pdp_type;             /*!< Packet Data Protocol (PDP) type specifies the type of data payload
	                                             exchanged over the airlink when the packet data session is
	                                             established with this profile. */ 
	ql_apn_auth_proto_e auth_proto;         /*!< Authentication Protocol. */
	char apn_name[QL_APN_NAME_SIZE];        /*!< A string parameter that is a logical name used to select the GGSN
	                                             and external packet data network. */ 
	char username[QL_APN_USERNAME_SIZE];    /*!< Username used during data network authentication. */
	char password[QL_APN_PASSWORD_SIZE];    /*!< Password to be used during data network authentication. */
} ql_apn_info_s;

typedef struct {
	ql_apn_pdp_type_e pdp_type;             /*!< Packet Data Protocol (PDP) type specifies the type of data payload
						  exchanged over the airlink when the packet data session is
						  established with this profile. */ 
	ql_apn_auth_proto_e auth_proto;         /*!< Authentication Protocol. */
	char apn_name[QL_APN_NAME_SIZE];        /*!< A string parameter that is a logical name used to select the GGSN
						  and external packet data network. */ 
	char username[QL_APN_USERNAME_SIZE];    /*!< Username used during data network authentication. */
	char password[QL_APN_PASSWORD_SIZE];    /*!< Password to be used during data network authentication. */
} ql_apn_add_s;

typedef struct {
	int cnt;
	ql_apn_info_s apn[QL_APN_MAX_LIST];
} ql_apn_info_list_s;

/**
 * Initialization data call module, and callback function registered.
 *
 * @param [in] evt_cb           callback fucntion
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Init(ql_data_call_evt_cb_t evt_cb);

/**
 * Destroy data call module, and unregister callback funciton
 *
 * @param 
 *   None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern void QL_Data_Call_Destroy(void);

/**
 * Starts a data call. If profile index is zero, it will call CDMA profile.
 *
 * @param [in] data_call        The data call parameters
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Start(ql_data_call_s *data_call, ql_data_call_error_e *err);

/**
 * Stop a data call.
 *
 * @param [in] profile_idx      UMTS/CDMA profile ID
 * @param [in] ip_family        IP Version
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Stop(char profile_idx, ql_data_call_ip_family_e ip_family, ql_data_call_error_e *err);

/**
 * Get a data call information.
 *
 * @param [in] profile_idx      UMTS/CDMA profile ID
 * @param [in] ip_family        IP Version
 * @param [out] info            The Data Call information
 * @param [out] error           Error code returned by data call 
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Info_Get(char profile_idx,
				 ql_data_call_ip_family_e ip_family,
				 ql_data_call_info_s *info,
				 ql_data_call_error_e *err);

/**
 * Set default profile information.
 *
 * @param [in] profile          The profile configured.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Set_Default_Profile(ql_data_call_default_profile_s *profile);

/**
 * Get default profile information.
 *
 * @param [in] profile          The profile configured.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_Data_Call_Get_Default_Profile(ql_data_call_default_profile_s *profile);

/**
 * Changes the settings in a configured profile. 
 *
 * @param [in] apn      the profile information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned, such apn not exist.
 * 
 */
extern int QL_APN_Set(ql_apn_info_s *apn);

/**
 * Retrieves the settings from a configured profile.
 *
 * @param [in] profile_idx        UMTS/CDMA profile ID
 * @param [out] apn               the profile information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_APN_Get(unsigned char profile_idx, ql_apn_info_s *apn);

/**
 * Retrieves the settings from a configured profile.
 *
 * @param [in] apn               the profile information.
 * @param [out] profile_idx        UMTS/CDMA profile ID
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_APN_Add(ql_apn_add_s *apn, unsigned char *profile_idx);

/**
 * Delete a configured profile.
 *
 * @param [in] profile_idx        UMTS/CDMA profile ID
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_APN_Del(unsigned char profile_idx);

/**
 * Retrieves the settings from a configured profile list.
 *
 * @param [out] apn_list          the profile list information.
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int QL_APN_Get_Lists(ql_apn_info_list_s *apn_list);

#define ql_data_call_init QL_Data_Call_Init
#define ql_data_call_destroy QL_Data_Call_Destroy
#define ql_data_call_start QL_Data_Call_Start
#define ql_data_call_stop QL_Data_Call_Stop
#define ql_data_call_info_get QL_Data_Call_Info_Get
#define ql_apn_set QL_APN_Set
#define ql_apn_get QL_APN_Get
#define ql_apn_add QL_APN_Add
#define ql_apn_del QL_APN_Del
#define ql_apn_get_lists QL_APN_Get_Lists

#endif /* end of __QL_WWAN_V2_H__ */

