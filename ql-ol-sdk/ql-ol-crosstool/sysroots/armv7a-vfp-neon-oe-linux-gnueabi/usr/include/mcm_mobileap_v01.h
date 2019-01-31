#ifndef MCM_MOBILEAP_SERVICE_01_H
#define MCM_MOBILEAP_SERVICE_01_H
/**
  @file mcm_mobileap_v01.h

  @brief This is the public header file which defines the mcm_mobileap service Data structures.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   Copyright (c) 2013 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#include <stdint.h>
#include "mcm_common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup mcm_mobileap_consts
    @{
  */
#define MCM_MOBILEAP_ENABLED_EV_V01 0x8000  /**< Enabled event. */
#define MCM_MOBILEAP_LAN_CONNECTING_EV_V01 0x8001  /**< LAN connecting. */
#define MCM_MOBILEAP_LAN_CONNECTING_FAIL_EV_V01 0x8002  /**< LAN connection failed. */
#define MCM_MOBILEAP_LAN_IPv6_CONNECTING_FAIL_EV_V01 0x8003  /**< LAN IPv6 connection failed. */
#define MCM_MOBILEAP_LAN_CONNECTED_EV_V01 0x8004  /**< LAN connected. */
#define MCM_MOBILEAP_STA_CONNECTED_EV_V01 0x8005  /**< Station connected. */
#define MCM_MOBILEAP_LAN_IPv6_CONNECTED_EV_V01 0x8006  /**< LAN IPv6 connected. */
#define MCM_MOBILEAP_WAN_CONNECTING_EV_V01 0x8007  /**< WAN connecting. */
#define MCM_MOBILEAP_WAN_CONNECTING_FAIL_EV_V01 0x8008  /**< WAN connection failed. */
#define MCM_MOBILEAP_WAN_IPv6_CONNECTING_FAIL_EV_V01 0x8009  /**< WAN IPv6 connection failed. */
#define MCM_MOBILEAP_WAN_CONNECTED_EV_V01 0x800A  /**< WAN connected. */
#define MCM_MOBILEAP_WAN_IPv6_CONNECTED_EV_V01 0x800B  /**< WAN IPv6 connected. */
#define MCM_MOBILEAP_WAN_DISCONNECTED_EV_V01 0x800C  /**< WAN disconnected. */
#define MCM_MOBILEAP_WAN_IPv6_DISCONNECTED_EV_V01 0x800D  /**< WAN IPv6 disconnected. */
#define MCM_MOBILEAP_LAN_DISCONNECTED_EV_V01 0x800E  /**< LAN disconnected. */
#define MCM_MOBILEAP_LAN_IPv6_DISCONNECTED_EV_V01 0x800F  /**< LAN IPv6 disconnected. */
#define MCM_MOBILEAP_DISABLED_EV_V01 0x8010  /**< Disabled event. */
#define MCM_MOBILEAP_MAX_FIREWALL_ENTRIES_V01 50  /**< Maximum firewall entries. */
#define MCM_MOBILEAP_MAX_STATIC_NAT_ENTRIES_V01 50  /**< Maximum static NAT entries. */

/**  IPv6 address length. */
#define MCM_MOBILEAP_IPV6_ADDR_LEN_V01 16

/**  MAC address length. */
#define MCM_MOBILEAP_MAC_ADDR_LEN_V01 6

/**  Maximum length of the device name. */
#define MCM_MOBILEAP_DEVICE_NAME_MAX_V01 100

/**  Maximum lease time length. */
#define MCM_MOBILEAP_LEASE_TIME_LEN_V01 100

/**  Maximum timeout for SYNC msgs (in milliseconds). */
#define MCM_MOBILEAP_MSG_TIMEOUT_VALUE_V01 90000
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_NAT_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_NAT_SYMMETRIC_NAT_V01 = 0,  /**< Symmetric NAT. */
  MCM_MOBILEAP_NAT_PORT_RESTRICTED_CONE_NAT_V01 = 1, /**< Port-restricted cone NAT. */
  MCM_MOBILEAP_NAT_FULL_CONE_NAT_V01 = 2,  /**< Full cone NAT (currently not supported). */
  MCM_MOBILEAP_NAT_ADDRESS_RESTRICTED_NAT_V01 = 3, /**< Address-restricted NAT (currently not supported). */
  MCM_MOBILEAP_NAT_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_nat_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_IP_FAMILY_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_IP_FAMILY_V4_V01 = 0x04,  /**< IP family v4. */
  MCM_MOBILEAP_IP_FAMILY_V6_V01 = 0x06,  /**< IP family v6 (currently not supported). */
  MCM_MOBILEAP_IP_FAMILY_V4V6_V01 = 0x0A,  /**< IP family v4/v6. */
  MCM_MOBILEAP_IP_FAMILY_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_ip_family_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_NAT_TIMEOUT_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_NAT_TIMEOUT_GENERIC_V01 = 0x01, /**< Generic NAT timeout. */
  MCM_MOBILEAP_NAT_TIMEOUT_ICMP_V01 = 0x02, /**< NAT timeout for ICMP (currently not supported). */
  MCM_MOBILEAP_NAT_TIMEOUT_TCP_ESTABLISHED_V01 = 0x03, /**< NAT timeout for the established TCP (currently not supported). */
  MCM_MOBILEAP_NAT_TIMEOUT_UDP_V01 = 0x04, /**<  NAT timeout for UDP (currently not supported). */
  MCM_MOBILEAP_NAT_TIMEOUT_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_nat_timeout_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_IP_VERSION_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_IP_V4_V01 = 4, /**< IPv4.  */
  MCM_MOBILEAP_IP_V6_V01 = 6, /**< IPv6. @newpage */
  MCM_MOBILEAP_IP_VERSION_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_ip_version_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_WWAN_STATUS_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_WWAN_STATUS_CONNECTING_V01 = 0x01, /**< IPv4 WWAN is in the Connecting state.  */
  MCM_MOBILEAP_WWAN_STATUS_CONNECTING_FAIL_V01 = 0x02, /**< IPv4 connection to the WWAN failed.  */
  MCM_MOBILEAP_WWAN_STATUS_CONNECTED_V01 = 0x03, /**< IPv4 WWAN is in the Connected state.  */
  MCM_MOBILEAP_WWAN_STATUS_DISCONNECTING_V01 = 0x04, /**< IPv4 WWAN is disconnecting.  */
  MCM_MOBILEAP_WWAN_STATUS_DISCONNECTING_FAIL_V01 = 0x05, /**< IPv4 WWAN failed to disconnect.  */
  MCM_MOBILEAP_WWAN_STATUS_DISCONNECTED_V01 = 0x06, /**< IPv4 WWAN is disconnected.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_CONNECTING_V01 = 0x07, /**< IPv6 WWAN is in the Connecting state.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_CONNECTING_FAIL_V01 = 0x08, /**< IPv6 connection to the WWAN failed.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_CONNECTED_V01 = 0x09, /**< IPv6 WWAN is in the Connected state.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_DISCONNECTING_V01 = 0x0A, /**< IPv6 WWAN is disconnecting.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_DISCONNECTING_FAIL_V01 = 0x0B, /**< IPv6 WWAN failed to disconnect.  */
  MCM_MOBILEAP_WWAN_STATUS_IPV6_DISCONNECTED_V01 = 0x0C, /**< IPv6 WWAN is disconnected.  */
  MCM_MOBILEAP_WWAN_STATUS_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_wwan_status_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_INVALID_V01 = -1, /**< Unknown.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_MOBILE_IP_V01 = 1, /**< Mobile IP.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_INTERNAL_V01 = 2, /**< Internal.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_CALL_MANAGER_DEFINED_V01 = 3, /**< Call manager-defined.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_3GPP_SPEC_DEFINED_V01 = 4, /**< 3GPP specification-defined.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_PPP_V01 = 5, /**< PPP.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_EHRPD_V01 = 6, /**< E-HRPD.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_IPV6_V01 = 7, /**< IPv6.  */
  MCM_MOBILEAP_WWAN_CALL_END_TYPE_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_wwan_call_end_type_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  mcm_mobileap_wwan_call_end_type_t_v01 wwan_call_end_reason_type;
  /**< WWAN call end type. */

  int32_t wwan_call_end_reason_code;
  /**< WWAN call end reason code. */
}mcm_mobileap_wwan_call_end_reason_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint32_t addr;
  /**< IPv4 address as specified in the IPv4 protocol
       specification (RFC 791 @xhyperref{S2,[S2]}).
  */

  uint32_t subnet_mask;
  /**< IPv4 subnet mask as specified in the IPv4
       protocol specification (RFC 791 @xhyperref{S2,[S2]}).
  */
}mcm_mobileap_ip4_addr_subnet_mask_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint8_t addr[MCM_MOBILEAP_IPV6_ADDR_LEN_V01];
  /**< IPv6 address as specified in the IPv6
       protocol specification (RFC 2460 @xhyperref{S5,[S5]}).
  */

  uint8_t prefix_len;
  /**< IPv6 prefix length as specified in the IPv6
       protocol specification (RFC 3513 @xhyperref{S6,[S6]}).
  */
}mcm_mobileap_ip6_addr_prefix_len_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint16_t port;
  /**< TCP/UDP port as specified in the
       TCP and UDP protocols (RFC 793 @xhyperref{S4,[S4]} and
       RFC 768 @xhyperref{S1,[S1]}).
  */

  uint16_t range;
  /**< TCP/UDP port range as specified in the
       TCP and UDP protocols (RFC 793 @xhyperref{S4,[S4]} and
       RFC 768 @xhyperref{S1,[S1]}).
  */
}mcm_mobileap_tcp_udp_port_range_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint8_t value;
  /**< TOS value as specified in the IPv4 protocol
       (RFC 791 @xhyperref{S2,[S2]}). */

  uint8_t mask;
  /**< IPv4 TOS mask */
}mcm_mobileap_ip4_tos_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint8_t value;
  /**< IPv6 traffic class value as specified in the IPv6 protocol
       (RFC 2460 @xhyperref{S5,[S5]}). */

  uint8_t mask;
  /**< IPv6 traffic class mask */
}mcm_mobileap_ip6_traffic_class_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint32_t port_fwding_private_ip;    /**< Port forwarding private IP. */
  /**< Private IP. */

  uint16_t port_fwding_private_port;  /**< Port forwarding private port. */
  /**< Private port. */

  uint16_t port_fwding_global_port;   /**< Port forwarding global IP. */
  /**< Global port. */

  uint8_t port_fwding_protocol;       /**< Port forwarding protocol. */
  /**< Protocol. */
}mcm_mobileap_static_nat_entry_conf_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint64_t bytes_rx;         /**< Number of Rx bytes. */
  /**< Number of Rx bytes. */

  uint64_t bytes_tx;         /**< Number of Tx bytes. */
  /**< Number of Tx bytes. */

  uint32_t pkts_rx;          /**< Number of Rx packets. */
  /**< Number of Rx packets. */

  uint32_t pkts_tx;          /**< Number of Tx packets. */
  /**< Number of Tx packets. */

  uint32_t pkts_dropped_rx;  /**< Number of dropped Rx packets. */
  /**< Number of Rx packets dropped.   */

  uint32_t pkts_dropped_tx;  /**< Number of dropped Tx packets. */
  /**< Number of Tx packets dropped. */
}mcm_mobileap_wwan_statistics_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint32_t intf;   /**< Interface. */
  /**< Interface. */

  uint32_t start;  /**< Start. */
  /**< Start. */

  uint32_t end;    /**< End. */
  /**< End. */

  char leasetime[MCM_MOBILEAP_LEASE_TIME_LEN_V01];  /**< Lease time length. */
  /**< Lease time length. */
}mcm_mobileap_dhcpd_config_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint8_t enable;          /**< Enable dual AP. */
  /**< Enable dual AP. */

  uint32_t a5_ip_address;  /**< A5 IP address. */
  /**< A5 IP address. */

  uint32_t sub_net_mask;   /**< Subnet mask. */
  /**< Subnet mask. */
}mcm_mobileap_dualap_config_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_enums
    @{
  */
typedef enum {
  MCM_MOBILEAP_STA_CONNECTION_T_MIN_ENUM_VAL_V01 = -2147483647, /* To force a 32 bit signed enum.  Do not change or use*/
  MCM_MOBILEAP_STA_CONNECTION_MIN_FORCE_TO_32_BIT_V01 = -2147483647,
  MCM_MOBILEAP_STA_CONNECTION_DYNAMIC_V01 = 0, /**< Dynamic station connection.  */
  MCM_MOBILEAP_STA_CONNECTION_STATIC_V01 = 1, /**< Static station connection.  */
  MCM_MOBILEAP_STA_CONNECTION_MAX_FORCE_TO_32_BIT_V01 = 2147483647,
  MCM_MOBILEAP_STA_CONNECTION_T_MAX_ENUM_VAL_V01 = 2147483647 /* To force a 32 bit signed enum.  Do not change or use*/
}mcm_mobileap_sta_connection_t_v01;
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  uint32_t ip_addr;   /**< IP address. */
  /**< IP address. */

  uint32_t gw_ip;     /**< GW IP. */
  /**< GSM/WCDMA IP. */

  uint32_t netmask;   /**< Net mask. */
  /**< Net mask. */

  uint32_t dns_addr;  /**< DNS address. */
  /**< DNS address. */
}mcm_mobileap_sta_static_ip_config_t_v01;  /* Type */
/**
    @}
  */

/** @addtogroup mcm_mobileap_aggregates
    @{
  */
typedef struct {

  mcm_mobileap_sta_connection_t_v01 conn_type;  /**< Connection type. */
  /**< Connection type. */

  mcm_mobileap_sta_static_ip_config_t_v01 static_ip_config;  /**< Static IP configuration. */
  /**< Static IP configuration. */
}mcm_mobileap_sta_connection_config_t_v01;  /* Type */
/**
    @}
  */

/** @cond
*/

typedef struct {
  /* This element is a placeholder to prevent the declaration of
     an empty struct.  DO NOT USE THIS FIELD UNDER ANY CIRCUMSTANCE */
  /*ASYNC Call*/
  char __placeholder;
}mcm_mobileap_enable_req_msg_v01;

/** @endcond */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Enables the mobile AP functionality via a single mobile AP instance
           on the \n ARM\reg Cortex\tmns-A5 processor. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  Mobile AP Handle */
  uint8_t mcm_mobileap_handle_valid;  /**< Must be set to TRUE if mcm_mobileap_handle is being passed. */
  uint32_t mcm_mobileap_handle;  /**< Mobile AP handle. */
}mcm_mobileap_enable_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Disables the mobile AP functionality for a mobile AP instance on
           the Cortex-A5 processor. */
  /*ASYNC Call*/
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_disable_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Disables the mobile AP functionality for a mobile AP instance on
           the Cortex-A5 processor. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_disable_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Invokes bringing up the WWAN from the mobile AP.

    The call is established using the stored network policy that enabled the
    mobile AP via MCM_MOBILEAP_ENABLE_REQ. If the control point issues multiple
    requests in short intervals, an MCM_ERROR_NO_EFFECT error is returned
    indicating that the previous request is still in process.*/
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Optional */
  uint8_t ip_version_valid;  /**< Must be set to TRUE if ip_version is being passed. */
  mcm_mobileap_ip_version_t_v01 ip_version;  /**< IP version. */
}mcm_mobileap_bring_up_wwan_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Invokes bringing up the WWAN from the mobile AP. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_bring_up_wwan_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Brings down a WWAN call, if connected.

    This command brings down the backhaul functionality. If the control point
    issues multiple requests in short intervals, an MCM_ERROR_NO_EFFECT error
    is returned indicating that the previous request is still in process.*/
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Optional */
  uint8_t ip_version_valid;  /**< Must be set to TRUE if ip_version is being passed. */
  mcm_mobileap_ip_version_t_v01 ip_version;  /**< IP version. */
}mcm_mobileap_tear_down_wwan_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Brings down a WWAN call, if connected. */
typedef struct {

  /* Optional */
  /*  Result Code */
  uint8_t resp_valid;  /**< Must be set to TRUE if resp is being passed. */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_tear_down_wwan_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Adds a static Network Address Translation (NAT) entry. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  SNAT Entry Configuration */
  mcm_mobileap_static_nat_entry_conf_t_v01 nat_entry_config;  /**< Mobile AP static NAT entry configuration. */
}mcm_mobileap_add_static_nat_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Adds a static NAT entry. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_add_static_nat_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries all static NAT entries. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Max SNAT Entries. */
  uint32_t max_entries;
  /**< Maximum number of SNAT entries requested by the client.
  */
}mcm_mobileap_get_static_nat_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries all static NAT entries. The response message
    contains the number of entries followed by the value of these entries
    sequentially. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  SNAT Configuration */
  uint8_t snat_entries_valid;  /**< Must be set to TRUE if snat_entries is being passed. */
  uint32_t snat_entries_len;  /**< Must be set to the number of elements in snat_entries. */
  mcm_mobileap_static_nat_entry_conf_t_v01 snat_entries[MCM_MOBILEAP_MAX_STATIC_NAT_ENTRIES_V01];
    /**< MCM mobile AP static NAT entry configuration. */
}mcm_mobileap_get_static_nat_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Deletes a static NAT entry. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  SNAT Entry Configuration */
  mcm_mobileap_static_nat_entry_conf_t_v01 snat_entry;  /**< MCM mobile AP static SNAT entry request message. */
}mcm_mobileap_delete_static_nat_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Deletes a static NAT entry. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. @newpagetable */
}mcm_mobileap_delete_static_nat_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures different types of NAT timeouts. The command
    handler overwrites any previously configured value with the current value. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  NAT Timeout Type */
  mcm_mobileap_nat_timeout_t_v01 timeout_type;
  /**< NAT timeout type to be used. Values: \n
      - MCM_MOBILEAP_NAT_TIMEOUT_GENERIC (0x01) --  Generic NAT timeout.
      - MCM_MOBILEAP_NAT_TIMEOUT_ICMP (0x02) --  NAT timeout for ICMP.
      - MCM_MOBILEAP_NAT_TIMEOUT_TCP_ESTABLISHED (0x03) --  NAT timeout for the established TCP.
      - MCM_MOBILEAP_NAT_TIMEOUT_UDP (0x04) --  NAT timeout for UDP. @tablebulletend
 */

  /* Mandatory */
  /*  NAT Timeout Value */
  uint32_t timeout_value;
  /**< NAT timeout value to be used, in seconds.
  */
}mcm_mobileap_set_nat_timeout_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures different types of NAT timeouts. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. @newpagetable */
}mcm_mobileap_set_nat_timeout_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the configured NAT timeout value. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  NAT Timeout Type */
  mcm_mobileap_nat_timeout_t_v01 timeout_type;
  /**< NAT timeout type used. Values: \n
      - MCM_MOBILEAP_NAT_TIMEOUT_GENERIC (0x01) --  Generic NAT timeout.
      - MCM_MOBILEAP_NAT_TIMEOUT_ICMP (0x02) --  NAT timeout for ICMP.
      - MCM_MOBILEAP_NAT_TIMEOUT_TCP_ESTABLISHED (0x03) --  NAT timeout for the established TCP.
      - MCM_MOBILEAP_NAT_TIMEOUT_UDP (0x04) --  NAT timeout for UDP. @tablebulletend
 */
}mcm_mobileap_get_nat_timeout_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the configured NAT timeout value. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;

  /* Optional */
  /*  NAT Timeout Value */
  uint8_t timeout_value_valid;  /**< Must be set to TRUE if timeout_value is being passed. */
  uint32_t timeout_value;
  /**< NAT timeout value used, in seconds. @newpagetable
  */
}mcm_mobileap_get_nat_timeout_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures the NAT type setting. The command
    handler overwrites any previously configured value with the current value. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
   */

  /* Mandatory */
  /*  NAT Type Value */
  mcm_mobileap_nat_type_t_v01 nat_type;
  /**< Type of NAT. Values: \n
      - MCM_MOBILEAP_NAT_SYMMETRIC_NAT (0) -- Symmetric NAT
      - MCM_MOBILEAP_NAT_PORT_RESTRICTED_CONE_NAT (1) -- Port-restricted cone NAT
      - MCM_MOBILEAP_NAT_FULL_CONE_NAT (2) -- Full cone NAT
      - MCM_MOBILEAP_NAT_ADDRESS_RESTRICTED_NAT (3) -- Address-restricted NAT @tablebulletend
 */
}mcm_mobileap_set_nat_type_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures the NAT type setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_nat_type_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the NAT type setting. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
    The value must be the handle previously returned by
    MCM_MOBILEAP_ENABLE_REQ. @newpagetable
   */
}mcm_mobileap_get_nat_type_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the NAT type setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  NAT Type Value */
  uint8_t nat_type_valid;  /**< Must be set to TRUE if nat_type is being passed. */
  mcm_mobileap_nat_type_t_v01 nat_type;
  /**< Type of NAT. Values: \n
      - MCM_MOBILEAP_NAT_SYMMETRIC_NAT (0) -- Symmetric NAT
      - MCM_MOBILEAP_NAT_PORT_RESTRICTED_CONE_NAT (1) -- Port-restricted cone NAT
      - MCM_MOBILEAP_NAT_FULL_CONE_NAT (2) -- Full cone NAT
      - MCM_MOBILEAP_NAT_ADDRESS_RESTRICTED_NAT (3) -- Address-restricted NAT @tablebulletend
 */
}mcm_mobileap_get_nat_type_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Adds IP filter-based firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
    */

  /* Mandatory */
  /*  IP Version */
  mcm_mobileap_ip_family_t_v01 ip_version;
  /**< Firewall family version. Values: \n
      - MCM_MOBILEAP_IP_FAMILY_V4 (0x04) -- IP family v4
      - MCM_MOBILEAP_IP_FAMILY_V6 (0x06) -- IP family v6
      - MCM_MOBILEAP_IP_FAMILY_V4V6 (0x0A) -- IP family v4/v6 @tablebulletend
 */

  /* Optional */
  /*  Next Protocol Header After the IP Header */
  uint8_t next_hdr_prot_valid;  /**< Must be set to TRUE if next_hdr_prot is being passed. */
  uint8_t next_hdr_prot;  /**< Next protocol header after the IP header. */
  /**< IPv4/IPv6 next header protocol. */

  /* Optional */
  /*  TCP, UDP, and TCP_UDP Source Port */
  uint8_t tcp_udp_src_valid;  /**< Must be set to TRUE if tcp_udp_src is being passed. */
  mcm_mobileap_tcp_udp_port_range_t_v01 tcp_udp_src;  /**< TCP_UDP source port. */
  /**< TCP, UDP, and TCP_UDP source port. */

  /* Optional */
  /*  TCP, UDP, and TCP_UDP Destination Port */
  uint8_t tcp_udp_dst_valid;  /**< Must be set to TRUE if tcp_udp_dst is being passed. */
  mcm_mobileap_tcp_udp_port_range_t_v01 tcp_udp_dst;  /**< TCP_UDP destination port. */
  /**< TCP, UDP, and TCP_UDP destination port. */

  /* Optional */
  /*  ICMP Type */
  uint8_t icmp_type_valid;  /**< Must be set to TRUE if icmp_type is being passed. */
  uint8_t icmp_type;
  /**< ICMP type, as specified in the ICMP protocol
         (RFC 792 @xhyperref{S3,[S3]}). */

  /* Optional */
  /*  ICMP Code */
  uint8_t icmp_code_valid;  /**< Must be set to TRUE if icmp_code is being passed. */
  uint8_t icmp_code;
  /**< ICMP code as specified in the ICMP protocol
         (RFC 792 @xhyperref{S3,[S3]}). */

  /* Optional */
  /*  ESP Security Parameter Index */
  uint8_t esp_spi_valid;  /**< Must be set to TRUE if esp_spi is being passed. */
  uint32_t esp_spi;
  /**< Security parameter index, as specified in the ESP protocol
        (RFC 4303 @xhyperref{S7,[S7]}). */

  /* Optional */
  /*  IPv4 Source Address and Subnet Mask */
  uint8_t ip4_src_addr_valid;  /**< Must be set to TRUE if ip4_src_addr is being passed. */
  mcm_mobileap_ip4_addr_subnet_mask_t_v01 ip4_src_addr;  /**< IPv4 source address and subnet mask. */
  /**< IPv4 Source address and subnet mask. */

  /* Optional */
  /*  IPv4 Destination Address and Subnet Mask */
  uint8_t ip4_dst_addr_valid;  /**< Must be set to TRUE if ip4_dst_addr is being passed. */
  mcm_mobileap_ip4_addr_subnet_mask_t_v01 ip4_dst_addr;  /**< IPv4 destination address and subnet mask. */
  /**< IPv4 destination address and subnet mask. */

  /* Optional */
  /*  IPv4 TOS Value and Mask */
  uint8_t ip4_tos_valid;  /**< Must be set to TRUE if ip4_tos is being passed. */
  mcm_mobileap_ip4_tos_t_v01 ip4_tos;  /**< IPv6 TOS value and mask. */
  /**< IPv4 TOS value and mask. */

  /* Optional */
  /*  IPv6 Source Address and Prefix Length */
  uint8_t ip6_src_addr_valid;  /**< Must be set to TRUE if ip6_src_addr is being passed. */
  mcm_mobileap_ip6_addr_prefix_len_t_v01 ip6_src_addr;
  /**< IPv6 source address and prefix length. */

  /* Optional */
  /*  IPv6 Destination Address and Prefix Length */
  uint8_t ip6_dst_addr_valid;  /**< Must be set to TRUE if ip6_dst_addr is being passed. */
  mcm_mobileap_ip6_addr_prefix_len_t_v01 ip6_dst_addr;  /**< IPv6 source address and prefix length. */
  /**< IPv6 destination address and prefix length. */

  /* Optional */
  /*  IPv6 Traffic Class Value and Mask */
  uint8_t ip6_trf_cls_valid;  /**< Must be set to TRUE if ip6_trf_cls is being passed. */
  mcm_mobileap_ip6_traffic_class_t_v01 ip6_trf_cls;  /**< IPv6 traffic class value and mask. */
  /**< IPv6 traffic class value and mask. */
}mcm_mobileap_add_firewall_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Adds IP filter-based firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  Firewall Handle */
  uint8_t firewall_handle_valid;  /**< Must be set to TRUE if firewall_handle is being passed. */
  uint32_t firewall_handle;
  /**< Identifies the handle for the added firewall rule. */
}mcm_mobileap_add_firewall_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the handles of all firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       Value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
   */

  /* Mandatory */
  /*  IP Version */
  mcm_mobileap_ip_family_t_v01 ip_version;
  /**< Identifies the firewall family version. Values: \n
      - MCM_MOBILEAP_IP_FAMILY_V4 (0x04) -- IP family v4
      - MCM_MOBILEAP_IP_FAMILY_V6 (0x06) -- IP family v6
      - MCM_MOBILEAP_IP_FAMILY_V4V6 (0x0A) -- IP family v4/v6 @tablebulletend
 */
}mcm_mobileap_get_firewall_entries_handle_list_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the handles of all firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  Firewall Handle List */
  uint8_t firewall_handle_list_valid;  /**< Must be set to TRUE if firewall_handle_list is being passed. */
  uint32_t firewall_handle_list_len;  /**< Must be set to the number of elements in firewall_handle_list. */
  uint32_t firewall_handle_list[MCM_MOBILEAP_MAX_FIREWALL_ENTRIES_V01];
  /**< Handles identifying the firewall entry. The value must be the handle
       previously returned by MCM_MOBILEAP_ADD_FIREWALL_ENTRY_RESP.
   */
}mcm_mobileap_get_firewall_entries_handle_list_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
   */

  /* Mandatory */
  /*  Firewall Handle */
  uint32_t firewall_handle;
  /**< Handle identifying the firewall entry. The value must be the handle
       previously returned by
       MCM_MOBILEAP_ADD_FIREWALL_ENTRY_RESP or
       MCM_MOBILEAP_GET_FIREWALL_ENTRIES_HANDLE_LIST_RESP.
   */
}mcm_mobileap_get_firewall_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the firewall rules. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  IP Version */
  uint8_t ip_version_valid;  /**< Must be set to TRUE if ip_version is being passed. */
  mcm_mobileap_ip_family_t_v01 ip_version;
  /**< Identifies the firewall family version. Values: \n
      - MCM_MOBILEAP_IP_FAMILY_V4 (0x04) -- IP family v4
      - MCM_MOBILEAP_IP_FAMILY_V6 (0x06) -- IP family v6
      - MCM_MOBILEAP_IP_FAMILY_V4V6 (0x0A) -- IP family v4/v6 @tablebulletend
 */

  /* Optional */
  /*  Next Header Protocol */
  uint8_t next_hdr_prot_valid;  /**< Must be set to TRUE if next_hdr_prot is being passed. */
  uint8_t next_hdr_prot;
  /**< IPv4/IPv6 next header protocol. */

  /* Optional */
  /*  TCP, UDP, and TCP_UDP Source Port */
  uint8_t tcp_udp_src_valid;  /**< Must be set to TRUE if tcp_udp_src is being passed. */
  mcm_mobileap_tcp_udp_port_range_t_v01 tcp_udp_src;  /**< TCP, UDP, and TCP_UDP source port. */
  /**< TCP, UDP, and TCP_UDP source port. */

  /* Optional */
  /*  TCP, UDP, and TCP_UDP Destination Port */
  uint8_t tcp_udp_dst_valid;  /**< Must be set to TRUE if tcp_udp_dst is being passed. */
  mcm_mobileap_tcp_udp_port_range_t_v01 tcp_udp_dst;  /**< TCP, UDP, and TCP_UDP destination port. */
  /**< TCP, UDP, and TCP_UDP destination port. */

  /* Optional */
  /*  ICMP Type */
  uint8_t icmp_type_valid;  /**< Must be set to TRUE if icmp_type is being passed. */
  uint8_t icmp_type;
  /**< ICMP type, as specified in the ICMP protocol
       (RFC 792 @xhyperref{S3,[S3]}). */

  /* Optional */
  /*  ICMP Code */
  uint8_t icmp_code_valid;  /**< Must be set to TRUE if icmp_code is being passed. */
  uint8_t icmp_code;
  /**< ICMP code, as specified in the ICMP protocol
       (RFC 792 @xhyperref{S3,[S3]}). */

  /* Optional */
  /*  ESP Security Parameter Index */
  uint8_t esp_spi_valid;  /**< Must be set to TRUE if esp_spi is being passed. */
  uint32_t esp_spi;
  /**< Security parameter index, as specified in the ESP protocol
       (RFC 4303 @xhyperref{S7,[S7]}). */

  /* Optional */
  /*  IPv4 Source Address and Subnet Mask */
  uint8_t ip4_src_addr_valid;  /**< Must be set to TRUE if ip4_src_addr is being passed. */
  mcm_mobileap_ip4_addr_subnet_mask_t_v01 ip4_src_addr;  /**<IPv4 source address and subnet mask. */
  /**< IPv4 source address and subnet mask. */

  /* Optional */
  /*  IPv4 Destination Address and Subnet Mask */
  uint8_t ip4_dst_addr_valid;  /**< Must be set to TRUE if ip4_dst_addr is being passed. */
  mcm_mobileap_ip4_addr_subnet_mask_t_v01 ip4_dst_addr;  /**< IPv4 destination address and subnet mask. */
  /**< IPv4 destination address and subnet mask. */

  /* Optional */
  /*  IPv4 TOS Value and Mask */
  uint8_t ip4_tos_valid;  /**< Must be set to TRUE if ip4_tos is being passed. */
  mcm_mobileap_ip4_tos_t_v01 ip4_tos;  /**< IPv4 TOS value and mask. */
  /**< IPv4 TOS value and mask. */

  /* Optional */
  /*  IPv6 Source Address and Prefix Length */
  uint8_t ip6_src_addr_valid;  /**< Must be set to TRUE if ip6_src_addr is being passed. */
  mcm_mobileap_ip6_addr_prefix_len_t_v01 ip6_src_addr;  /**< IPv6 source address and prefix length. */
  /**< IPv6 source address and prefix length. */

  /* Optional */
  /*  IPv6 Destination Address and Prefix Length */
  uint8_t ip6_dst_addr_valid;  /**< Must be set to TRUE if ip6_dst_addr is being passed. */
  mcm_mobileap_ip6_addr_prefix_len_t_v01 ip6_dst_addr;  /**< IPv6 destination address and prefix length. */
  /**< IPv6 destination address and prefix length. */

  /* Optional */
  /*  IPv6 Traffic Class Value and Mask */
  uint8_t ip6_trf_cls_valid;  /**< Must be set to TRUE if ip6_trf_cls is being passed. */
  mcm_mobileap_ip6_traffic_class_t_v01 ip6_trf_cls;  /**< IPv6 traffic class value and mask. */
  /**< IPv6 traffic class value and mask. @newpagetable */
}mcm_mobileap_get_firewall_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Deletes a firewall rule identified by a handle. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
         Value must be the handle previously returned by
         MCM_MOBILEAP_ENABLE_REQ.
    */

  /* Mandatory */
  /*  Firewall Handle */
  uint32_t firewall_handle;
  /**< Handle identifying the firewall entry.
    */
}mcm_mobileap_delete_firewall_entry_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Deletes a firewall rule identified by a handle. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_delete_firewall_entry_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Sets the firewall configuration.

   This command enables or disables the firewall. If the firewall is enabled,
   it sets the firewall state to accept or drop the packets.*/
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Firewall Enabled */
  uint8_t firewall_enabled;
  /**< Indicates whether the firewall is to be enabled or disabled; a Boolean value.
  */

  /* Optional */
  /*  Accept or Drop Packets */
  uint8_t pkts_allowed_valid;  /**< Must be set to TRUE if pkts_allowed is being passed. */
  uint8_t pkts_allowed;
  /**< Indicates whether packets are to be accepted or dropped; a Boolean value. @newpagetable
  */
}mcm_mobileap_set_firewall_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Sets the firewall configuration. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_firewall_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Sets the DMZ (perimeter network) IP address for the mobile AP. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  DMZ IP Address */
  uint32_t dmz_ip_addr;
  /**< DMZ IP address. */
}mcm_mobileap_add_dmz_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Sets the DMZ (perimeter network) IP address for the mobile AP. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_add_dmz_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries the DMZ IP address on the mobile AP. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ. @newpagetable
  */
}mcm_mobileap_get_dmz_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries the DMZ IP address on the mobile AP. If no DMZ
    is set by the client, an IP address of 0.0.0.0 is returned. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  DMZ IP Address */
  uint8_t dmz_ip_addr_valid;  /**< Must be set to TRUE if dmz_ip_addr is being passed. */
  uint32_t dmz_ip_addr;
  /**< DMZ IP address. */
}mcm_mobileap_get_dmz_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Deletes the DMZ entry or DMZ IP address. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  DMZ IP Address */
  uint32_t dmz_ip_addr;  /**< DMZ IP address. */
}mcm_mobileap_delete_dmz_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Deletes the DMZ entry or DMZ IP address. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_delete_dmz_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries the WWAN IP configuration. The command must be
    issued by the control point after MCM_MOBILEAP_WWAN_STATUS_IND has
    indicated success in bringing up a WWAN, otherwise an MCM_ERROR_INTERNAL
    error is returned. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_get_ipv4_wwan_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries the WWAN IP configuration. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  IPv4 Address */
  uint8_t v4_addr_valid;  /**< Must be set to TRUE if v4_addr is being passed. */
  uint32_t v4_addr;
  /**< IPv4 address. */

  /* Optional */
  /*  IPv4 Primary DNS Address */
  uint8_t v4_prim_dns_addr_valid;  /**< Must be set to TRUE if v4_prim_dns_addr is being passed. */
  uint32_t v4_prim_dns_addr;
  /**< IPv4 primary DNS address. */

  /* Optional */
  /*  IPv4 Secondary DNS Address */
  uint8_t v4_sec_dns_addr_valid;  /**< Must be set to TRUE if v4_sec_dns_addr is being passed. */
  uint32_t v4_sec_dns_addr;
  /**< IPv4 secondary DNS address. */
}mcm_mobileap_get_ipv4_wwan_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets WWAN statistics. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  IP Family */
  mcm_mobileap_ip_family_t_v01 ip_family;
  /**< Identifies the IP version to be used. Values: \n
      - MCM_MOBILEAP_IP_FAMILY_V4 (0x04) -- IP family v4
      - MCM_MOBILEAP_IP_FAMILY_V6 (0x06) -- IP family v6
      - MCM_MOBILEAP_IP_FAMILY_V4V6 (0x0A) -- IP family v4/v6 @tablebulletend
 */
}mcm_mobileap_get_wwan_stats_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets WWAN statistics. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  WWAN Statistics */
  uint8_t wwan_stats_valid;  /**< Must be set to TRUE if wwan_stats is being passed. */
  mcm_mobileap_wwan_statistics_t_v01 wwan_stats;  /**< WWAN statistics.   */
  /**< WWAN statistics. */
}mcm_mobileap_get_wwan_stats_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Resets WWAN statistics. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  WWAN Entry With IP Version Configured */
  mcm_mobileap_ip_family_t_v01 ip_family;
  /**< Identifies the IP version to be used. Values: \n
      - MCM_MOBILEAP_IP_FAMILY_V4 (0x04) -- IP family v4 
      - MCM_MOBILEAP_IP_FAMILY_V6 (0x06) -- IP family v6
      - MCM_MOBILEAP_IP_FAMILY_V4V6 (0x0A) -- IP family v4/v6 @tablebulletend
 */
}mcm_mobileap_reset_wwan_stats_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Resets WWAN statistics. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_reset_wwan_stats_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Sets the DHCPD configuration. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  mcm_mobileap_dhcpd_config_t_v01 dhcpd_config;  /**< DHCPD configuration.   */
  /**< DHCPD configuration. */
}mcm_mobileap_set_dhcpd_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Sets the DHCPD configuration. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_dhcpd_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Enables the WLAN. */
  /*ASYNC Call*/
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_enable_wlan_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Enables the WLAN. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_enable_wlan_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Disables the WLAN. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_MOBILEAP_ENABLE_REQ. @newpagetable 
  */
  /*ASYNC Call*/
}mcm_mobileap_disable_wlan_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Disables the WLAN. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_disable_wlan_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures the Internet Protocol Security (IPSec)
           Virtual Private Network (VPN) passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value;
  /**< Indicates whether an IPSec VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_set_ipsec_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures the Internet Protocol Security (IPSec)
    Virtual Private Network (VPN) passthrough setting. The command handler
    overwrites any previously configured value with the current value. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_ipsec_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries the IPSec VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_get_ipsec_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries the IPSec VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value_valid;  /**< Must be set to TRUE if vpn_pass_through_value is being passed. */
  uint8_t vpn_pass_through_value;
  /**< Indicates whether an IPSec VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_get_ipsec_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures the Point-to-Point Tunneling Protocol (PPTP)
    VPN passthrough setting. The command handler overwrites any previously
    configured value with the current value. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value;
  /**< Indicates whether a PPTP VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_set_pptp_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures the Point-to-Point Tunneling Protocol (PPTP) VPN
           passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_pptp_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries the PPTP VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_get_pptp_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries the PPTP VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value_valid;  /**< Must be set to TRUE if vpn_pass_through_value is being passed. */
  uint8_t vpn_pass_through_value;  /**< VPN passthrough value. */
  /**< Indicates whether a PPTP VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_get_pptp_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures the Layer 2 Tunneling Protocol (L2TP) VPN
    passthrough setting. The command handler overwrites any previously
    configured value with the current value. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value;
  /**< Indicates whether an L2TP VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_set_l2tp_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures the Layer 2 Tunneling Protocol (L2TP) VPN passthrough
           setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_l2tp_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Queries the L2TP VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ. @newpagetable
  */
}mcm_mobileap_get_l2tp_vpn_pass_through_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Queries the L2TP VPN passthrough setting. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  VPN Passthrough Value */
  uint8_t vpn_pass_through_value_valid;  /**< Must be set to TRUE if vpn_pass_through_value is being passed. */
  uint8_t vpn_pass_through_value;
  /**< Indicates whether an L2TP VPN passthrough is allowed; a Boolean value.
  */
}mcm_mobileap_get_l2tp_vpn_pass_through_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Sets the autoconnect flag. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Autoconnect Mode Flag */
  uint8_t enable;
  /**< Enable/disable autoconnect. Values:\n
      - TRUE -- Enable\n
      - FALSE -- Disable @tablebulletend
   */
}mcm_mobileap_set_auto_connect_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Sets the autoconnect flag. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_auto_connect_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the autoconnect flag. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_get_auto_connect_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the autoconnect flag. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  Autoconnect Mode */
  uint8_t auto_conn_flag_valid;  /**< Must be set to TRUE if auto_conn_flag is being passed. */
  uint8_t auto_conn_flag;
  /**< Autoconnect status. Values: \n
       - TRUE -- Enabled \n
       - FALSE -- Disabled @tablebulletend
   */
}mcm_mobileap_get_auto_connect_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures whether QCMAP_MSGR initiates WWAN data calls
    while roaming.

    The roaming mode determines the QCMAP_MSGR policy for establishing new data
    calls. By default, this is assumed to be FALSE. If modified through this
    interface, it is stored persistently.
    
    @latexonly \textbf{Note:} @endlatexonly

    The roaming mode does not affect a currently established data connection.
    For example, if the roaming mode is set to FALSE, but a roaming data call
    is connected (e.g., by a different client or because the mode was TRUE
    when the call was established), QCMAP_MSGR uses the currently established
    WWAN data connection. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Roaming Mode */
  uint8_t allow_wwan_calls_while_roaming;
  /**< Roaming mode. Indicates whether QCMAP_MSGR connects a data call while
       roaming; a Boolean value. 
  */
}mcm_mobileap_set_roaming_pref_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures whether QCMAP_MSGR initiates WWAN data calls while roaming. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_roaming_pref_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Gets the roaming flag. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_get_roaming_pref_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Gets the roaming flag. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */

  /* Optional */
  /*  Roaming Mode */
  uint8_t allow_wwan_calls_while_roaming_valid;  /**< Must be set to TRUE if allow_wwan_calls_while_roaming is being passed. */
  uint8_t allow_wwan_calls_while_roaming;
  /**< Determines whether the mobile AP connects a data call while roaming; a Boolean value.
  */
}mcm_mobileap_get_roaming_pref_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Configures whether the mobile AP initiates WWAN data calls while roaming. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_MSGR_MOBILE_AP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Dual AP Config */
  mcm_mobileap_dualap_config_t_v01 dualap_config;
  /**< Mobile AP dual SSID configuration. @newpagetable
  */
}mcm_mobileap_set_dualap_config_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Configures whether the mobile AP initiates WWAN data calls while roaming. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_set_dualap_config_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Enables Station (STA) mode functionality for a mobile AP
   instance on the modem.
   
   After this request is successfully processed, all packet connectivity to
   an outside network occurs through the WLAN station. The modem routing
   engine appropriately handles the packet routing into and out of the modem. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */

  /* Mandatory */
  /*  Station Mode Configuration. */
  mcm_mobileap_sta_connection_config_t_v01 cfg;
  /**< Station mode configuration to indicate dynamic or static IP
       configuration.
  */
}mcm_mobileap_station_mode_enable_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Enables Station (STA) mode functionality for a mobile AP instance
           on the modem. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. @newpagetable */
}mcm_mobileap_station_mode_enable_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Disables STA mode functionality for a mobile AP instance
    on the  modem. When this request has been successfully processed, the
    control point invokes bringing up the WWAN from the mobile AP if auto-connect
    is enabled. */
typedef struct {

  /* Mandatory */
  /*  Mobile AP Handle */
  uint32_t mcm_mobileap_handle;
  /**< Handle identifying the mobile AP call instance.
       The value must be the handle previously returned by
       MCM_MOBILEAP_ENABLE_REQ.
  */
}mcm_mobileap_station_mode_disable_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Disables STA mode functionality for a mobile AP instance on the
           modem. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  mcm_response_t_v01 resp;  /**< Result code. */
}mcm_mobileap_station_mode_disable_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Registers for an indication of events. */
typedef struct {

  /* Optional */
  uint8_t register_event_enabled_valid;  /**< Must be set to TRUE if register_event_enabled is being passed. */
  uint8_t register_event_enabled;  /**< Event registration is enabled. */

  /* Optional */
  uint8_t register_event_lan_connecting_valid;  /**< Must be set to TRUE if register_event_lan_connecting is being passed. */
  uint8_t register_event_lan_connecting;  /**< Register for a LAN connecting event. */

  /* Optional */
  uint8_t register_event_lan_connecting_fail_valid;  /**< Must be set to TRUE if register_event_lan_connecting_fail is being passed. */
  uint8_t register_event_lan_connecting_fail;  /**< Register for a LAN connection failure event. */

  /* Optional */
  uint8_t register_event_lan_ipv6_connecting_fail_valid;  /**< Must be set to TRUE if register_event_lan_ipv6_connecting_fail is being passed. */
  uint8_t register_event_lan_ipv6_connecting_fail;  /**< Register for a LAN IPv6 connection failure event. */

  /* Optional */
  uint8_t register_event_lan_connected_valid;  /**< Must be set to TRUE if register_event_lan_connected is being passed. */
  uint8_t register_event_lan_connected;  /**< Register for a LAN connected event. */

  /* Optional */
  uint8_t register_event_sta_connected_valid;  /**< Must be set to TRUE if register_event_sta_connected is being passed. */
  uint8_t register_event_sta_connected;  /**< Register for a STA connected event. */

  /* Optional */
  uint8_t register_event_lan_ipv6_connected_valid;  /**< Must be set to TRUE if register_event_lan_ipv6_connected is being passed. */
  uint8_t register_event_lan_ipv6_connected;  /**< Register for a LAN IPv6 connected event. */

  /* Optional */
  uint8_t register_event_wan_connecting_valid;  /**< Must be set to TRUE if register_event_wan_connecting is being passed. */
  uint8_t register_event_wan_connecting;  /**< Register for a WAN connecting event. */

  /* Optional */
  uint8_t register_event_wan_connecting_fail_valid;  /**< Must be set to TRUE if register_event_wan_connecting_fail is being passed. */
  uint8_t register_event_wan_connecting_fail;  /**< Register for a WAN connection failure event. */

  /* Optional */
  uint8_t register_event_wan_ipv6_connecting_fail_valid;  /**< Must be set to TRUE if register_event_wan_ipv6_connecting_fail is being passed. */
  uint8_t register_event_wan_ipv6_connecting_fail;  /**< Register for a WAN IPv6 connection failure event. */

  /* Optional */
  uint8_t register_event_wan_connected_valid;  /**< Must be set to TRUE if register_event_wan_connected is being passed. */
  uint8_t register_event_wan_connected;  /**< Register for a WAN connected event. */

  /* Optional */
  uint8_t register_event_wan_ipv6_connected_valid;  /**< Must be set to TRUE if register_event_wan_ipv6_connected is being passed. */
  uint8_t register_event_wan_ipv6_connected;  /**< Register for a WAN IPv6 connected event. */

  /* Optional */
  uint8_t register_event_wan_disconnected_valid;  /**< Must be set to TRUE if register_event_wan_disconnected is being passed. */
  uint8_t register_event_wan_disconnected;  /**< Register for a WAN disconnected event. */

  /* Optional */
  uint8_t register_event_wan_ipv6_disconnected_valid;  /**< Must be set to TRUE if register_event_wan_ipv6_disconnected is being passed. */
  uint8_t register_event_wan_ipv6_disconnected;  /**< Register for a WAN IPv6 disconnected event. */

  /* Optional */
  uint8_t register_event_lan_disconnected_valid;  /**< Must be set to TRUE if register_event_lan_disconnected is being passed. */
  uint8_t register_event_lan_disconnected;  /**< Register for a LAN disconnected event. */

  /* Optional */
  uint8_t register_event_lan_ipv6_disconnected_valid;  /**< Must be set to TRUE if register_event_lan_ipv6_disconnected is being passed. */
  uint8_t register_event_lan_ipv6_disconnected;  /**< Register for a LAN IPv6 disconnected event. */

  /* Optional */
  uint8_t register_event_disabled_valid;  /**< Must be set to TRUE if register_event_disabled is being passed. */
  uint8_t register_event_disabled;  /**< Event registration is disabled. @newpagetable */
}mcm_mobileap_event_register_req_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Response message; Registers for an indication of events. */
typedef struct {

  /* Mandatory */
  mcm_response_t_v01 response;  /**< Result code. */
}mcm_mobileap_event_register_resp_msg_v01;  /* Message */
/**
    @}
  */

/** @addtogroup mcm_mobileap_messages
    @{
  */
/** Request message; Indication corresponding to a registered unsolicited event. */
typedef struct {

  /* Mandatory */
  int32_t event_id;
  /**<   Event ID thats gets populated. Values: \n
        - 0x8000L -- MCM_MOBILEAP_ENABLED_EV
        - 0x8001L -- MCM_MOBILEAP_LAN_CONNECTING_EV
        - 0x8002L -- MCM_MOBILEAP_LAN_CONNECTING_FAIL_EV
        - 0x8003L -- MCM_MOBILEAP_LAN_IPv6_CONNECTING_FAIL_EV
        - 0x8004L -- MCM_MOBILEAP_LAN_CONNECTED_EV
        - 0x8005L -- MCM_MOBILEAP_STA_CONNECTED_EV
        - 0x8006L -- MCM_MOBILEAP_LAN_IPv6_CONNECTED_EV
        - 0x8007L -- MCM_MOBILEAP_WAN_CONNECTING_EV
        - 0x8008L -- MCM_MOBILEAP_WAN_CONNECTING_FAIL_EV
        - 0x8009L -- MCM_MOBILEAP_WAN_IPv6_CONNECTING_FAIL_EV
        - 0x800AL -- MCM_MOBILEAP_WAN_CONNECTED_EV
        - 0x800BL -- MCM_MOBILEAP_WAN_IPv6_CONNECTED_EV
        - 0x800CL -- MCM_MOBILEAP_WAN_DISCONNECTED_EV
        - 0x800DL -- MCM_MOBILEAP_WAN_IPv6_DISCONNECTED_EV
        - 0x800EL -- MCM_MOBILEAP_LAN_DISCONNECTED_EV
        - 0x800FL -- MCM_MOBILEAP_LAN_IPv6_DISCONNECTED_EV
        - 0x8010L -- MCM_MOBILEAP_DISABLED_EV @tablebulletend
  */
}mcm_mobileap_unsol_event_ind_msg_v01;  /* Message */
/**
    @}
  */


/*Service Message Definition*/
/** @addtogroup mcm_mobileap_msg_ids
    @{
  */

/** @name
@{ */

#define MCM_MOBILEAP_ENABLE_REQ_V01 0x0400
#define MCM_MOBILEAP_ENABLE_RESP_V01 0x0400
#define MCM_MOBILEAP_DISABLE_REQ_V01 0x0401
#define MCM_MOBILEAP_DISABLE_RESP_V01 0x0401
#define MCM_MOBILEAP_BRING_UP_WWAN_REQ_V01 0x0402
#define MCM_MOBILEAP_BRING_UP_WWAN_RESP_V01 0x0402
#define MCM_MOBILEAP_TEAR_DOWN_WWAN_REQ_V01 0x0403
#define MCM_MOBILEAP_TEAR_DOWN_WWAN_RESP_V01 0x0403
#define MCM_MOBILEAP_ADD_STATIC_NAT_ENTRY_REQ_V01 0x0404
#define MCM_MOBILEAP_ADD_STATIC_NAT_ENTRY_RESP_V01 0x0404
#define MCM_MOBILEAP_GET_STATIC_NAT_ENTRY_REQ_V01 0x0405
#define MCM_MOBILEAP_GET_STATIC_NAT_ENTRY_RESP_V01 0x0405
#define MCM_MOBILEAP_DELETE_STATIC_NAT_ENTRY_REQ_V01 0x0406
#define MCM_MOBILEPA_DELETE_STATIC_NAT_ENTRY_RESP_V01 0x0406
#define MCM_MOBILEAP_SET_NAT_TIMEOUT_REQ_V01 0x0407
#define MCM_MOBILEAP_SET_NAT_TIMEOUT_RESP_V01 0x0407
#define MCM_MOBILEAP_GET_NAT_TIMEOUT_REQ_V01 0x0408
#define MCM_MOBILEAP_GET_NAT_TIMEOUT_RESP_V01 0x0408
#define MCM_MOBILEAP_SET_NAT_TYPE_REQ_V01 0x0409
#define MCM_MOBILEAP_SET_NAT_TYPE_RESP_V01 0x0409
#define MCM_MOBILEAP_GET_NAT_TYPE_REQ_V01 0x040A
#define MCM_MOBILEAP_GET_NAT_TYPE_RESP_V01 0x040A
#define MCM_MOBILEAP_ADD_FIREWALL_ENTRY_REQ_V01 0x040B
#define MCM_MOBILEAP_ADD_FIREWALL_ENTRY_RESP_V01 0x040B
#define MCM_MOBILEAP_GET_FIREWALL_ENTRIES_HANDLE_LIST_REQ_V01 0x040C
#define MCM_MOBILEAP_GET_FIREWALL_ENTRIES_HANDLE_LIST_RESP_V01 0x040C
#define MCM_MOBILEAP_GET_FIREWALL_ENTRY_REQ_V01 0x040D
#define MCM_MOBILEAP_GET_FIREWALL_ENTRY_RESP_V01 0x040D
#define MCM_MOBILEAP_DELETE_FIREWALL_ENTRY_REQ_V01 0x040E
#define MCM_MOBILEAP_DELETE_FIREWALL_ENTRY_RESP_V01 0x040E
#define MCM_MOBILEAP_SET_FIREWALL_CONFIG_REQ_V01 0x040F
#define MCM_MOBILEAP_SET_FIREWALL_CONFIG_RESP_V01 0x040F
#define MCM_MOBILEAP_SET_DMZ_REQ_V01 0x0410
#define MCM_MOBILEAP_SET_DMZ_RESP_V01 0x0410
#define MCM_MOBILEAP_DELETE_DMZ_REQ_V01 0x0411
#define MCM_MOBILEAP_DELETE_DMZ_RESP_V01 0x0411
#define MCM_MOBILEAP_GET_DMZ_REQ_V01 0x0412
#define MCM_MOBILEAP_GET_DMZ_RESP_V01 0x0412
#define MCM_MOBILEAP_GET_IPV4_WWAN_CONFIG_REQ_V01 0x0413
#define MCM_MOBILEAP_GET_IPV4_WWAN_CONFIG_RESP_V01 0x0413
#define MCM_MOBILEAP_GET_WWAN_STATS_REQ_V01 0x0414
#define MCM_MOBILEAP_GET_WWAN_STATS_RESP_V01 0x0414
#define MCM_MOBILEAP_RESET_WWAN_STATS_REQ_V01 0x0415
#define MCM_MOBILEAP_RESET_WWAN_STATS_RESP_V01 0x0415
#define MCM_MOBILEAP_SET_DHCPD_CONFIG_REQ_V01 0x0416
#define MCM_MOBILEAP_SET_DHCPD_CONFIG_RESP_V01 0x0416
#define MCM_MOBILEAP_ENABLE_WLAN_REQ_V01 0x0417
#define MCM_MOBILEAP_ENABLE_WLAN_RESP_V01 0x0417
#define MCM_MOBILEAP_DISABLE_WLAN_REQ_V01 0x0418
#define MCM_MOBILEAP_DISABLE_WLAN_RESP_V01 0x0418
#define MCM_MOBILEAP_GET_IPSEC_VPN_PASS_THROUGH_REQ_V01 0x0419
#define MCM_MOBILEAP_GET_IPSEC_VPN_PASS_THROUGH_RESP_V01 0x0419
#define MCM_MOBILEAP_SET_IPSEC_VPN_PASS_THROUGH_REQ_V01 0x041A
#define MCM_MOBILEAP_SET_IPSEC_VPN_PASS_THROUGH_RESP_V01 0x041A
#define MCM_MOBILEAP_GET_PPTP_VPN_PASS_THROUGH_REQ_V01 0x041B
#define MCM_MOBILEAP_GET_PPTP_VPN_PASS_THROUGH_RESP_V01 0x041B
#define MCM_MOBILEAP_SET_PPTP_VPN_PASS_THROUGH_REQ_V01 0x041C
#define MCM_MOBILEAP_SET_PPTP_VPN_PASS_THROUGH_RESP_V01 0x041C
#define MCM_MOBILEAP_GET_L2TP_VPN_PASS_THROUGH_REQ_V01 0x041D
#define MCM_MOBILEAP_GET_L2TP_VPN_PASS_THROUGH_RESP_V01 0x041D
#define MCM_MOBILEAP_SET_L2TP_VPN_PASS_THROUGH_REQ_V01 0x041E
#define MCM_MOBILEAP_SET_L2TP_VPN_PASS_THROUGH_RESP_V01 0x041E
#define MCM_MOBILEAP_SET_AUTO_CONNECT_REQ_V01 0x041F
#define MCM_MOBILEAP_SET_AUTO_CONNECT_RESP_V01 0x041F
#define MCM_MOBILEAP_GET_AUTO_CONNECT_REQ_V01 0x0420
#define MCM_MOBILEAP_GET_AUTO_CONNECT_RESP_V01 0x0420
#define MCM_MOBILEAP_SET_ROAMING_PREF_REQ_V01 0x0421
#define MCM_MOBILEAP_SET_ROAMING_PREF_RESP_V01 0x0421
#define MCM_MOBILEAP_GET_ROAMING_PREF_REQ_V01 0x0422
#define MCM_MOBILEAP_GET_ROAMING_PREF_RESP_V01 0x0422
#define MCM_MOBILEAP_SET_DUALAP_CONFIG_REQ_V01 0x0423
#define MCM_MOBILEAP_SET_DUALAP_CONFIG_RESP_V01 0x0423
#define MCM_MOBILEAP_STATION_MODE_ENABLE_REQ_V01 0x0424
#define MCM_MOBILEAP_STATION_MODE_ENABLE_RESP_V01 0x0424
#define MCM_MOBILEAP_STATION_MODE_DISABLE_REQ_V01 0x0425
#define MCM_MOBILEAP_STATION_MODE_DISABLE_RESP_V01 0x0425
#define MCM_MOBILEAP_EVENT_REGISTER_REQ_V01 0x0426
#define MCM_MOBILEAP_EVENT_REGISTER_RESP_V01 0x0426
#define MCM_MOBILEAP_UNSOL_EVENT_IND_V01 0x0427

/** @} */ /* end_namegroup */

/**
    @} */ /* end_namegroup */
/** @} */

#ifdef __cplusplus
}
#endif
#endif

