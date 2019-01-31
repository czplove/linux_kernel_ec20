#ifndef __QL_MCM_WWAN_H__
#define __QL_MCM_WWAN_H__



#define QL_MOBAP_MSG_TIMEOUT_VALUE      90000   /**  Maximum timeout for SYNC msgs (in milliseconds). */
#define QL_MOBAP_MAX_STATIC_NAT_ENTRIES 50      /**< Maximum static NAT entries. */

typedef uint32 mobap_client_handle_type;


typedef enum 
{
    E_QL_MOBILEAP_UNKNOWN_EV                    = -1,      /**< Unknown event. */
    E_QL_MOBILEAP_ENABLED_EV                    = 0x8000,  /**< Enabled event. */
    E_QL_MOBILEAP_LAN_CONNECTING_EV             = 0x8001,  /**< LAN connecting. */
    E_QL_MOBILEAP_LAN_CONNECTING_FAIL_EV        = 0x8002,  /**< LAN connection failed. */
    E_QL_MOBILEAP_LAN_IPv6_CONNECTING_FAIL_EV   = 0x8003,  /**< LAN IPv6 connection failed. */
    E_QL_MOBILEAP_LAN_CONNECTED_EV              = 0x8004,  /**< LAN connected. */
    E_QL_MOBILEAP_STA_CONNECTED_EV              = 0x8005,  /**< Station connected. */
    E_QL_MOBILEAP_LAN_IPv6_CONNECTED_EV         = 0x8006,  /**< LAN IPv6 connected. */
    E_QL_MOBILEAP_WAN_CONNECTING_EV             = 0x8007,  /**< WAN connecting. */
    E_QL_MOBILEAP_WAN_CONNECTING_FAIL_EV        = 0x8008,  /**< WAN connection failed. */
    E_QL_MOBILEAP_WAN_IPv6_CONNECTING_FAIL_EV   = 0x8009,  /**< WAN IPv6 connection failed. */
    E_QL_MOBILEAP_WAN_CONNECTED_EV              = 0x800A,  /**< WAN connected. */
    E_QL_MOBILEAP_WAN_IPv6_CONNECTED_EV         = 0x800B,  /**< WAN IPv6 connected. */
    E_QL_MOBILEAP_WAN_DISCONNECTED_EV           = 0x800C,  /**< WAN disconnected. */
    E_QL_MOBILEAP_WAN_IPv6_DISCONNECTED_EV      = 0x800D,  /**< WAN IPv6 disconnected. */
    E_QL_MOBILEAP_LAN_DISCONNECTED_EV           = 0x800E,  /**< LAN disconnected. */
    E_QL_MOBILEAP_LAN_IPv6_DISCONNECTED_EV      = 0x800F,  /**< LAN IPv6 disconnected. */
    E_QL_MOBILEAP_DISABLED_EV                   = 0x8010   /**< Disabled event. */
}E_QL_MOBILEAP_IND_EVENT_TYPE_T;

typedef void (*QL_MobAP_RxIndNotifyFunc_t)   
(    
    mobap_client_handle_type        h_mobap,
    E_QL_MOBILEAP_IND_EVENT_TYPE_T  e_msg_id,
    void                            *pv_data,
    void                            *contextPtr    
);

typedef enum 
{
    E_QL_MOBILEAP_IP_TYPE_V4    = 4,    /**< IPv4.  */
    E_QL_MOBILEAP_IP_TYPE_V6    = 6,    /**< IPv6.  */
    E_QL_MOBILEAP_IP_TYPE_V4V6  = 10,   /**< IPv4v6.  */
}E_QL_MOBILEAP_IP_TYPE_T;

typedef struct 
{
    uint32_t    port_fwding_private_ip;    /**< Port forwarding private IP. */
    uint16_t    port_fwding_private_port;  /**< Port forwarding private port. */
    uint16_t    port_fwding_global_port;   /**< Port forwarding global IP. */
    uint8_t     port_fwding_protocol;      /**< Port forwarding protocol. */
}QL_MOBAP_STATIC_NAT_ENTRY_CFG_T;  /* Type */

/** Adds a static Network Address Translation (NAT) entry. */
typedef struct 
{
    //uint32_t mcm_mobileap_handle; /**< Handle identifying the mobile AP call instance. The value must be the 
    //                                    handle previously returned by MCM_MOBILEAP_ENABLE_REQ.*/
    QL_MOBAP_STATIC_NAT_ENTRY_CFG_T nat_entry_config;  /**< Mobile AP static NAT entry configuration. */
}QL_MOBAP_STATIC_NAT_ENTRY_INFO_T;  /* Message */


typedef struct 
{
    uint32_t                            snat_entries_len;                               /**< Must be set to the number of elements in snat_entries. */
    QL_MOBAP_STATIC_NAT_ENTRY_CFG_T     snat_entries[QL_MOBAP_MAX_STATIC_NAT_ENTRIES];  /**< Mobile AP static NAT entry configuration. */
}QL_MOBAP_STATIC_NAT_ENTRY_LIST_T;


typedef enum 
{
    E_QL_MOBAP_NAT_TIMEOUT_TYPE_GENERIC         = 0x01, /**< Generic NAT timeout. */
    E_QL_MOBAP_NAT_TIMEOUT_TYPE_ICMP            = 0x02, /**< NAT timeout for ICMP (currently not supported). */
    E_QL_MOBAP_NAT_TIMEOUT_TYPE_TCP_ESTABLISHED = 0x03, /**< NAT timeout for the established TCP (currently not supported). */
    E_QL_MOBAP_NAT_TIMEOUT_TYPE_UDP             = 0x04, /**<  NAT timeout for UDP (currently not supported). */
}E_QL_MOBAP_NAT_TIMEOUT_TYPE_T;


typedef enum 
{
    E_QL_MOBAP_NAT_TYPE_SYMMETRIC               = 0,    /**< Symmetric NAT. */
    E_QL_MOBAP_NAT_TYPE_PORT_RESTRICTED_CONE    = 1,    /**< Port-restricted cone NAT. */
    E_QL_MOBAP_NAT_TYPE_FULL_CONE_NAT           = 2,    /**< Full cone NAT (currently not supported). */
    E_QL_MOBAP_NAT_TYPE_ADDRESS_RESTRICTED_NAT  = 3,    /**< Address-restricted NAT (currently not supported). */
}E_QL_MOBAP_NAT_TYPE_T;

typedef struct 
{
    uint16_t port;          /**< TCP/UDP port as specified in the TCP and UDP protocols (RFC 793 @xhyperref{S4,[S4]} and RFC 768 @xhyperref{S1,[S1]}). */
    uint16_t range;         /**< TCP/UDP port range as specified in the TCP and UDP protocols (RFC 793 @xhyperref{S4,[S4]} and RFC 768 @xhyperref{S1,[S1]}). */
}QL_MOBAP_TCP_UDP_PORT_RANGE_T;  /* Type */

typedef struct 
{
    uint32_t addr;          /**< IPv4 address as specified in the IPv4 protocol specification (RFC 791 @xhyperref{S2,[S2]}).  */
    uint32_t subnet_mask;   /**< IPv4 subnet mask as specified in the IPv4 protocol specification (RFC 791 @xhyperref{S2,[S2]}).  */
}QL_MOBAP_IPV4_ADDR_SUBNET_MASK_T;  /* Type */

typedef struct 
{
    uint8_t value;          /**< TOS value as specified in the IPv4 protocol(RFC 791 @xhyperref{S2,[S2]}). */
    uint8_t mask;           /**< IPv4 TOS mask */
}QL_MOBAP_IPV4_TOS_INFO_T;

#define QL_MOBAP_IPV6_ADDR_LEN 16
typedef struct 
{
    uint8_t prefix_len;                     /**< IPv6 prefix length as specified in the IPv6 protocol specification (RFC 3513 @xhyperref{S6,[S6]}).  */
    uint8_t addr[QL_MOBAP_IPV6_ADDR_LEN];   /**< IPv6 address as specified in the IPv6 protocol specification (RFC 2460 @xhyperref{S5,[S5]}).  */
}QL_MOBAP_IPV6_ADDR_PREFIX_LEN_INFO_T;

typedef struct 
{
    uint8_t value;      /**< IPv6 traffic class value as specified in the IPv6 protocol (RFC 2460 @xhyperref{S5,[S5]}). */
    uint8_t mask;       /**< IPv6 traffic class mask */
}QL_MOBAP_IPV6_TRAFFIC_CLASS_INFO_T;

typedef enum 
{
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_ICMP     = 1,    /**< ICMP */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_TCP      = 6,    /**< TCP */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_UDP      = 17,   /**< UDP */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_ESP      = 50,   /**< ESP */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_ICMP6    = 58,   /**< ICMP6 */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_TCP_UDP  = 253,  /**< TCP_UDP */
}E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_T;

/** IP filter-based firewall rules. */
typedef struct 
{
    E_QL_MOBILEAP_IP_TYPE_T ip_version;       /**< Firewall family version. */

    uint8_t next_hdr_prot_valid;              /**< Must be set to TRUE if next_hdr_prot is being passed. */
    E_QL_MOBAP_NEXT_HEAD_PROT_TYPE_T e_next_hdr_prot;       /**< Next protocol header after the IP header. */

    uint8_t tcp_udp_src_valid;                /**< Must be set to TRUE if tcp_udp_src is being passed. */
    QL_MOBAP_TCP_UDP_PORT_RANGE_T           tcp_udp_src;    /**< TCP_UDP source port. */

    uint8_t tcp_udp_dst_valid;                /**< Must be set to TRUE if tcp_udp_dst is being passed. */
    QL_MOBAP_TCP_UDP_PORT_RANGE_T           tcp_udp_dst;    /**< TCP_UDP destination port. */

    uint8_t icmp_type_valid;                  /**< Must be set to TRUE if icmp_type is being passed. */
    uint8_t icmp_type;                        /**< ICMP type, as specified in the ICMP protocol (RFC 792 @xhyperref{S3,[S3]}). */

    uint8_t icmp_code_valid;                  /**< Must be set to TRUE if icmp_code is being passed. */
    uint8_t icmp_code;                        /**< ICMP code as specified in the ICMP protocol (RFC 792 @xhyperref{S3,[S3]}). */

    uint8_t esp_spi_valid;                    /**< Must be set to TRUE if esp_spi is being passed. */
    uint32_t esp_spi;                         /**< Security parameter index, as specified in the ESP protocol (RFC 4303 @xhyperref{S7,[S7]}). */

    uint8_t ip4_src_addr_valid;               /**< Must be set to TRUE if ip4_src_addr is being passed. */
    QL_MOBAP_IPV4_ADDR_SUBNET_MASK_T        ip4_src_addr;  /**< IPv4 source address and subnet mask. */

    uint8_t ip4_dst_addr_valid;               /**< Must be set to TRUE if ip4_dst_addr is being passed. */
    QL_MOBAP_IPV4_ADDR_SUBNET_MASK_T        ip4_dst_addr;  /**< IPv4 destination address and subnet mask. */

    uint8_t ip4_tos_valid;                    /**< Must be set to TRUE if ip4_tos is being passed. */
    QL_MOBAP_IPV4_TOS_INFO_T                ip4_tos;       /**< IPv6 TOS value and mask. */

    uint8_t ip6_src_addr_valid;               /**< Must be set to TRUE if ip6_src_addr is being passed. */
    QL_MOBAP_IPV6_ADDR_PREFIX_LEN_INFO_T    ip6_src_addr;/**< IPv6 source address and prefix length. */

    uint8_t ip6_dst_addr_valid;               /**< Must be set to TRUE if ip6_dst_addr is being passed. */
    QL_MOBAP_IPV6_ADDR_PREFIX_LEN_INFO_T    ip6_dst_addr;  /**< IPv6 source address and prefix length. */

    uint8_t ip6_trf_cls_valid;                /**< Must be set to TRUE if ip6_trf_cls is being passed. */
    QL_MOBAP_IPV6_TRAFFIC_CLASS_INFO_T      ip6_trf_cls;  /**< IPv6 traffic class value and mask. */
}QL_MOBAP_FIREWALL_ENTRY_INFO_T;  /* Message */


/** WWAN IP configuration. */
typedef struct 
{
    uint8_t     v4_addr_valid;          /**< Must be set to TRUE if v4_addr is being passed. */
    uint32_t    v4_addr;                /**< IPv4 address. */
    uint8_t     v4_prim_dns_addr_valid; /**< Must be set to TRUE if v4_prim_dns_addr is being passed. */
    uint32_t    v4_prim_dns_addr;       /**< IPv4 primary DNS address. */
    uint8_t     v4_sec_dns_addr_valid;  /**< Must be set to TRUE if v4_sec_dns_addr is being passed. */
    uint32_t    v4_sec_dns_addr;        /**< IPv4 secondary DNS address. */
}QL_MOBAP_IPV4_WWAN_CONFIG_INFO_T;

typedef struct 
{
    uint64_t    bytes_rx;               /**< Number of Rx bytes. */
    uint64_t    bytes_tx;               /**< Number of Tx bytes. */
    uint32_t    pkts_rx;                /**< Number of Rx packets. */
    uint32_t    pkts_tx;                /**< Number of Tx packets. */
    uint32_t    pkts_dropped_rx;        /**< Number of dropped Rx packets. */
    uint32_t    pkts_dropped_tx;        /**< Number of dropped Tx packets. */
}QL_MOBAP_WWAN_STATISTICS_DATA_T;

typedef struct 
{
    uint8_t                             wwan_stats_valid;   /**< Must be set to TRUE if wwan_stats is being passed. */
    QL_MOBAP_WWAN_STATISTICS_DATA_T     wwan_stats;         /**< WWAN statistics.   */
}QL_MOBAP_WWAN_STATISTICS_INFO_T;

#define QL_MOBAP_LEASE_TIME_LEN     100
typedef struct 
{
    uint32_t    intf;   /**< Interface. */
    uint32_t    start;  /**< Start. */
    uint32_t    end;    /**< End. */
    char        leasetime[QL_MOBAP_LEASE_TIME_LEN];  /**< Lease time length. Lease time format is a positive decimal number 
                                                            followed by h for hours, m for minutes, or nothing for seconds.
                                                            Or, can be infinite for an infinite lease time. i.e. 3h10m*/
}QL_MOBAP_DHCP_CFG_INFO_T;


typedef enum 
{
    E_QL_MOBAP_STA_CONNECTION_TYPE_DYNAMIC  = 0, /**< Dynamic station connection.  */
    E_QL_MOBAP_STA_CONNECTION_TYPE_STATIC   = 1, /**< Static station connection.  */
}E_QL_MOBAP_STA_CONNECTION_TYPE_T;

typedef struct 
{
    uint32_t    ip_addr;    /**< IP address. */
    uint32_t    gw_ip;      /**< GSM/WCDMA IP. */
    uint32_t    netmask;    /**< Net mask. */
    uint32_t    dns_addr;   /**< DNS address. */
}QL_MOBAP_STA_STATIC_IP_CFG_INFO_T;

typedef struct 
{
    E_QL_MOBAP_STA_CONNECTION_TYPE_T    e_conn_type;        /**< Connection type. */
    QL_MOBAP_STA_STATIC_IP_CFG_INFO_T   static_ip_config;   /**< Static IP configuration. */
}QL_MOBAP_STA_CONNECTION_CFG_INFO_T;


typedef struct 
{
    uint8_t     register_event_enabled;                     /**< Event registration is enabled. */
    uint8_t     register_event_lan_connecting;              /**< Register for a LAN connecting event. */
    uint8_t     register_event_lan_connecting_fail;         /**< Register for a LAN connection failure event. */
    uint8_t     register_event_lan_ipv6_connecting_fail;    /**< Register for a LAN IPv6 connection failure event. */
    uint8_t     register_event_lan_connected;               /**< Register for a LAN connected event. */
    uint8_t     register_event_sta_connected;               /**< Register for a STA connected event. */
    uint8_t     register_event_lan_ipv6_connected;          /**< Register for a LAN IPv6 connected event. */
    uint8_t     register_event_wan_connecting;              /**< Register for a WAN connecting event. */
    uint8_t     register_event_wan_connecting_fail;         /**< Register for a WAN connection failure event. */
    uint8_t     register_event_wan_ipv6_connecting_fail;    /**< Register for a WAN IPv6 connection failure event. */
    uint8_t     register_event_wan_connected;               /**< Register for a WAN connected event. */
    uint8_t     register_event_wan_ipv6_connected;          /**< Register for a WAN IPv6 connected event. */
    uint8_t     register_event_wan_disconnected;            /**< Register for a WAN disconnected event. */
    uint8_t     register_event_wan_ipv6_disconnected;       /**< Register for a WAN IPv6 disconnected event. */
    uint8_t     register_event_lan_disconnected;            /**< Register for a LAN disconnected event. */
    uint8_t     register_event_lan_ipv6_disconnected;       /**< Register for a LAN IPv6 disconnected event. */
    uint8_t     register_event_disabled;                    /**< Event registration is disabled. @newpagetable */
}QL_MOBAP_EVENT_REG_INFO_T;



E_QL_ERROR_CODE_T QL_MCM_MobileAP_Init(mobap_client_handle_type  *ph_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_AddRxIndMsgHandler(QL_MobAP_RxIndNotifyFunc_t handlerPtr, void* contextPtr);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_Deinit(mobap_client_handle_type  h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_Enable(mobap_client_handle_type  h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_Disable(mobap_client_handle_type  h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_ConnectBackhaul(  mobap_client_handle_type    h_mobap,
                                                    E_QL_MOBILEAP_IP_TYPE_T     e_ip_version);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DisconnectBackhaul(   mobap_client_handle_type    h_mobap,
                                                        E_QL_MOBILEAP_IP_TYPE_T     e_ip_version);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_AddStaticNATEntry(mobap_client_handle_type         h_mobap,
                                                    QL_MOBAP_STATIC_NAT_ENTRY_INFO_T *pt_nat_info);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetStaticNATEntry(mobap_client_handle_type         h_mobap,
                                                    uint32_t                         ui4_max_entry,
                                                    QL_MOBAP_STATIC_NAT_ENTRY_LIST_T *pt_nat_list);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DeleteStaticNATEntry( mobap_client_handle_type         h_mobap,
                                                        QL_MOBAP_STATIC_NAT_ENTRY_CFG_T *pt_nat_entry);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetNATTimeout(mobap_client_handle_type         h_mobap,
                                                E_QL_MOBAP_NAT_TIMEOUT_TYPE_T    e_timeout_type,
                                                uint32_t                         timeout);//should be more than 30 s


E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetNATTimeout(mobap_client_handle_type         h_mobap,
                                                E_QL_MOBAP_NAT_TIMEOUT_TYPE_T    e_timeout_type,
                                                uint32_t                         *p_timeout);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetNATType(mobap_client_handle_type         h_mobap,
                                             E_QL_MOBAP_NAT_TYPE_T            e_nat_type);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetNATType(mobap_client_handle_type         h_mobap,
                                             E_QL_MOBAP_NAT_TYPE_T            *pe_nat_type);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_AddFirewallEntry( mobap_client_handle_type         h_mobap,
                                                    QL_MOBAP_FIREWALL_ENTRY_INFO_T   *pt_fw_entry);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DeleteFirewallEntry(mobap_client_handle_type        h_mobap,
                                                      E_QL_MOBILEAP_IP_TYPE_T         e_ip_version,
                                                      uint8_t                         deleted_entry_idx);// 1 based

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetFirewallEntry(mobap_client_handle_type        h_mobap,
                                                   E_QL_MOBILEAP_IP_TYPE_T         e_ip_version,
                                                   uint8_t                         get_entry_idx, // 1 base
                                                   QL_MOBAP_FIREWALL_ENTRY_INFO_T  *pt_fw_entry);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetFirewall(mobap_client_handle_type        h_mobap,
                                              uint8_t                         enable_firewall,
                                              uint8_t                         pkts_allowed);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_AddDMZ(mobap_client_handle_type        h_mobap,
                                         uint32_t                        dmz_ip_addr);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DeleteDMZ(mobap_client_handle_type    h_mobap,
                                            uint32_t                    dmz_ip_addr);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetDMZ(mobap_client_handle_type    h_mobap,
                                         uint32_t                    *p_dmz_ip_addr);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetIPv4WWANCfg(mobap_client_handle_type            h_mobap,
                                                 QL_MOBAP_IPV4_WWAN_CONFIG_INFO_T    *pt_wwan_cfg);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetWWANStatistics(mobap_client_handle_type        h_mobap,
                                                    E_QL_MOBILEAP_IP_TYPE_T         e_ip_version,
                                                    QL_MOBAP_WWAN_STATISTICS_INFO_T *pt_info);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_ResetWWANStatistics(mobap_client_handle_type        h_mobap,
                                                      E_QL_MOBILEAP_IP_TYPE_T         e_ip_version);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetDHCPDConfig(mobap_client_handle_type        h_mobap,
                                                 QL_MOBAP_DHCP_CFG_INFO_T        *pt_info);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_EnableWLAN(mobap_client_handle_type        h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DisableWLAN(mobap_client_handle_type  h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetIPSecVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                         uint8_t                    vpn_pass_through_enable);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetPPTPVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                        uint8_t                    vpn_pass_through_enable);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetL2TPVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                        uint8_t                    vpn_pass_through_enable);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetIPSecVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                         uint8_t                    *p_vpn_pass_through_value);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetPPTPVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                        uint8_t                    *p_vpn_pass_through_value);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetL2TPVPNPassthrough(mobap_client_handle_type   h_mobap,
                                                        uint8_t                    *p_vpn_pass_through_value);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetAutoconnect(mobap_client_handle_type   h_mobap,
                                                 uint8_t                    auto_connect_enable);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetRoaming(mobap_client_handle_type   h_mobap,
                                             uint8_t                    roaming_enable);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetAutoconnect(mobap_client_handle_type   h_mobap,
                                                 uint8_t                    *p_auto_connect_value);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_GetRoaming(mobap_client_handle_type   h_mobap,
                                             uint8_t                    *p_roaming_value);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_SetDUALAPConfig(mobap_client_handle_type   h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_EnableSTAMode(mobap_client_handle_type            h_mobap,
                                                uint8_t                             is_new_cfg, //0: means drop pt_info, 1:use pt_info setting
                                                QL_MOBAP_STA_CONNECTION_CFG_INFO_T  *pt_info);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_DisableSTAMode(mobap_client_handle_type   h_mobap);

E_QL_ERROR_CODE_T QL_MCM_MobileAP_EventRegister(mobap_client_handle_type    h_mobap,
                                                QL_MOBAP_EVENT_REG_INFO_T   *pt_reg_info);

#endif //__QL_MCM_WWAN_H__

