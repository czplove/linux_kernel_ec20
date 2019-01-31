#include <ql_oe.h>
#include "test_base.h"

extern func_api_test_t t_mobap_test;

st_api_test_case at_mobap_testlist[] = 
{
    {0,     "QL_MCM_MobileAP_Init"},
    {1,     "QL_MCM_MobileAP_AddRxIndMsgHandler"},
    {2,     "QL_MCM_MobileAP_EventRegister"},
    {3,     "QL_MCM_MobileAP_SetAutoconnect"},
    {4,     "QL_MCM_MobileAP_Enable"},
    {5,     "QL_MCM_MobileAP_ConnectBackhaul"},
    {6,     "QL_MCM_MobileAP_Disable"},
    {7,     "QL_MCM_MobileAP_Deinit"},
    {8,     "QL_MCM_MobileAP_GetAutoconnect"},
    {9,     "QL_MCM_MobileAP_DisconnectBackhaul"},
    {10,    "QL_MCM_MobileAP_AddStaticNATEntry"},
    {11,    "QL_MCM_MobileAP_GetStaticNATEntry"},
    {12,    "QL_MCM_MobileAP_DeleteStaticNATEntry"},
    {13,    "QL_MCM_MobileAP_SetNATTimeout"},
    {14,    "QL_MCM_MobileAP_GetNATTimeout"},
    {15,    "QL_MCM_MobileAP_SetNATType"},
    {16,    "QL_MCM_MobileAP_GetNATType"},
    {17,    "QL_MCM_MobileAP_AddFirewallEntry"},
    {18,    "QL_MCM_MobileAP_DeleteFirewallEntry"},
    {19,    "QL_MCM_MobileAP_GetFirewallEntry"},
    {20,    "QL_MCM_MobileAP_SetFirewall"},
    {21,    "QL_MCM_MobileAP_AddDMZ"},
    {22,    "QL_MCM_MobileAP_DeleteDMZ"},
    {23,    "QL_MCM_MobileAP_GetDMZ"},
    {24,    "QL_MCM_MobileAP_GetIPv4WWANCfg"},
    {25,    "QL_MCM_MobileAP_GetWWANStatistics"},
    {26,    "QL_MCM_MobileAP_ResetWWANStatistics"},
    {27,    "QL_MCM_MobileAP_SetDHCPDConfig"},
    {28,    "QL_MCM_MobileAP_EnableWLAN"},
    {29,    "QL_MCM_MobileAP_DisableWLAN"},
    {30,    "QL_MCM_MobileAP_SetIPSecVPNPassthrough"},
    {31,    "QL_MCM_MobileAP_SetPPTPVPNPassthrough"},
    {32,    "QL_MCM_MobileAP_SetL2TPVPNPassthrough"},
    {33,    "QL_MCM_MobileAP_GetIPSecVPNPassthrough"},
    {34,    "QL_MCM_MobileAP_GetPPTPVPNPassthrough"},
    {35,    "QL_MCM_MobileAP_GetL2TPVPNPassthrough"},
    {36,    "QL_MCM_MobileAP_SetRoaming"},
    {37,    "QL_MCM_MobileAP_GetRoaming"},
    {38,    "QL_MCM_MobileAP_SetDUALAPConfig"},
    {39,    "QL_MCM_MobileAP_EnableSTAMode"},
    {40,    "QL_MCM_MobileAP_DisableSTAMode"},
    
    {-1,    NULL}
};


static void handle_sig(int sig)
{
    printf("######## got signal %d #######\n", sig);
}

static void ql_mcm_mobileap_nfy
(
    mobap_client_handle_type        h_mobap,
    E_QL_MOBILEAP_IND_EVENT_TYPE_T  e_msg_id,
    void                            *pv_data,
    void                            *contextPtr    
)
{
    switch(e_msg_id)
    {
    case E_QL_MOBILEAP_ENABLED_EV:
        printf("\nEnabled Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_CONNECTING_EV:
        printf("\nLan Connecting Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_CONNECTING_FAIL_EV:
        printf("\nLan Connecting Fail Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_IPv6_CONNECTING_FAIL_EV:
        printf("\nLan IPv6 Connecting Fail Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_CONNECTED_EV:
        printf("\nLan Connected Event Received\n");
        break;

    case E_QL_MOBILEAP_STA_CONNECTED_EV:
        printf("\nStation Connected Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_IPv6_CONNECTED_EV:
        printf("\nLan IPv6 Connected Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_CONNECTING_EV:
        printf("\nWan Connecting Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_CONNECTING_FAIL_EV:
        printf("\nWan Connecting Fail Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_IPv6_CONNECTING_FAIL_EV:
        printf("\nWan IPv6 Connecting Fail Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_CONNECTED_EV:
        printf("\nWan Connected Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_IPv6_CONNECTED_EV:
        printf("\nWan IPv6 Connected Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_DISCONNECTED_EV:
        printf("\nWan Disconnected Event Received\n");
        break;

    case E_QL_MOBILEAP_WAN_IPv6_DISCONNECTED_EV:
        printf("\nWan IPv6 Disconnected Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_DISCONNECTED_EV:
        printf("\nLan Disconnected Event Received\n");
        break;

    case E_QL_MOBILEAP_LAN_IPv6_DISCONNECTED_EV:
        printf("\nLan IPv6 Disconnected Event Received\n");
        break;

    case E_QL_MOBILEAP_DISABLED_EV:
        printf("\nDisabled Event Received\n");
        break;

    default:
        printf("\nUnknown Event: 0x%x\n", e_msg_id);
        break;
    }
}


static int test_mobap(void)
{
    int     cmdIdx  = 0;
    int     h_mobap = 0;
    E_QL_ERROR_CODE_T ret;
    
    signal(SIGPIPE, handle_sig);
    
    show_group_help(&t_mobap_test);

    while(1)
    {
        printf("please input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }
        switch(cmdIdx)
        {
        case 0://"QL_MCM_MobileAP_Init"
            ret = QL_MCM_MobileAP_Init(&h_mobap);
            printf("QL_MCM_MobileAP_Init ret = %d, h_mobap=0x%X\n", ret, h_mobap);
            break;

        case 1://"QL_MCM_MobileAP_AddRxIndMsgHandler"
            ret = QL_MCM_MobileAP_AddRxIndMsgHandler(ql_mcm_mobileap_nfy, (void*)0x12345678);
            printf("QL_MCM_MobileAP_AddRxIndMsgHandler ret = %d\n", ret);
            break;
            
        case 2://"QL_MCM_MobileAP_EventRegister"
        {
            QL_MOBAP_EVENT_REG_INFO_T   t_reg_info;

            t_reg_info.register_event_enabled                   = 1;
            t_reg_info.register_event_lan_connecting            = 1;
            t_reg_info.register_event_lan_connecting_fail       = 1;
            t_reg_info.register_event_lan_ipv6_connecting_fail  = 1;
            t_reg_info.register_event_lan_connected             = 1;
            t_reg_info.register_event_sta_connected             = 1;
            t_reg_info.register_event_lan_ipv6_connected        = 1;
            t_reg_info.register_event_wan_connecting            = 1;
            t_reg_info.register_event_wan_connecting_fail       = 1;
            t_reg_info.register_event_wan_ipv6_connecting_fail  = 1;
            t_reg_info.register_event_wan_connected             = 1;
            t_reg_info.register_event_wan_ipv6_connected        = 1;
            t_reg_info.register_event_wan_disconnected          = 1;
            t_reg_info.register_event_wan_ipv6_disconnected     = 1;
            t_reg_info.register_event_lan_disconnected          = 1;
            t_reg_info.register_event_lan_ipv6_disconnected     = 1;
            t_reg_info.register_event_disabled                  = 1;
            
            ret = QL_MCM_MobileAP_EventRegister(h_mobap, &t_reg_info);
            printf("QL_MCM_MobileAP_EventRegister ret = %d\n", ret);
            break;
        }
            
        case 3://"QL_MCM_MobileAP_SetAutoconnect"
        {
            int enable = 0;
            printf("please input auto-connect flag(0: disable, 1:enable): ");
            scanf("%d", &enable);
            ret = QL_MCM_MobileAP_SetAutoconnect(h_mobap, (enable != 0));
            printf("QL_MCM_MobileAP_SetAutoconnect(%d) ret = %d\n", (enable != 0), ret);
            break;
        }
            
        case 4://"QL_MCM_MobileAP_Enable"
            ret = QL_MCM_MobileAP_Enable(h_mobap);
            printf("QL_MCM_MobileAP_Enable ret = %d\n", ret);
            break;

        case 5://"QL_MCM_MobileAP_ConnectBackhaul"
            ret = QL_MCM_MobileAP_ConnectBackhaul(h_mobap, E_QL_MOBILEAP_IP_TYPE_V4);
            printf("QL_MCM_MobileAP_ConnectBackhaul ret = %d\n", ret);
            break;
            
        case 6://"QL_MCM_MobileAP_Disable"
            ret = QL_MCM_MobileAP_Disable(h_mobap);
            printf("QL_MCM_MobileAP_Disable ret = %d\n", ret);
            break;
            
        case 7://"QL_MCM_MobileAP_Deinit"
            ret = QL_MCM_MobileAP_Deinit(h_mobap);
            printf("QL_MCM_MobileAP_Deinit ret = %d\n", ret);
            break;
            
        case 8://"QL_MCM_MobileAP_GetAutoconnect"
        {
            uint8_t auto_connect_val = 0;
            ret = QL_MCM_MobileAP_GetAutoconnect(h_mobap, &auto_connect_val);
            printf("QL_MCM_MobileAP_GetAutoconnect ret = %d, auto_connect_val= %d\n", ret, auto_connect_val);
            break;
        }
            
        case 9://"QL_MCM_MobileAP_DisconnectBackhaul"
            ret = QL_MCM_MobileAP_DisconnectBackhaul(h_mobap, E_QL_MOBILEAP_IP_TYPE_V4);
            printf("QL_MCM_MobileAP_DisconnectBackhaul ret = %d\n", ret);
            break;
            
        case 10://"QL_MCM_MobileAP_AddStaticNATEntry"
        {
            QL_MOBAP_STATIC_NAT_ENTRY_INFO_T t_nat_info;
            //This is just a example, you should modify it to the right value!
            t_nat_info.nat_entry_config.port_fwding_private_ip  = 0xC0A80001;//192.168.0.1
            t_nat_info.nat_entry_config.port_fwding_private_port= 0x1234;
            t_nat_info.nat_entry_config.port_fwding_global_port = 0x4321;
            t_nat_info.nat_entry_config.port_fwding_protocol    = 123;
           
            ret = QL_MCM_MobileAP_AddStaticNATEntry(h_mobap, &t_nat_info);
            printf("QL_MCM_MobileAP_AddStaticNATEntry ret = %d\n", ret);
            break;
        }
            
        case 11://"QL_MCM_MobileAP_GetStaticNATEntry"
        {
            int             i;
            struct in_addr  addr;
            QL_MOBAP_STATIC_NAT_ENTRY_LIST_T    t_nat_list;

            ret = QL_MCM_MobileAP_GetStaticNATEntry(h_mobap, 5, &t_nat_list);
            printf("QL_MCM_MobileAP_GetStaticNATEntry ret = %d, NAT list:\n", ret);
            for(i=0; i<t_nat_list.snat_entries_len; i++)
            {
                addr.s_addr = ntohl(t_nat_list.snat_entries[i].port_fwding_private_ip);
                printf("#%d: private_ip: %s\n"
                        "\tprivate_port: %d\n"
                        "\tglobal_port : %d\n"
                        "\tprotocol    : %d\n",
                        i+1,
                        inet_ntoa(addr),
                        t_nat_list.snat_entries[i].port_fwding_private_port,
                        t_nat_list.snat_entries[i].port_fwding_global_port,
                        t_nat_list.snat_entries[i].port_fwding_protocol);
            }              
            break;
        }
            
        case 12://"QL_MCM_MobileAP_DeleteStaticNATEntry"
        {
            QL_MOBAP_STATIC_NAT_ENTRY_CFG_T t_nat_info;
            
            //This is just a example, you should modify it to the right value!
            t_nat_info.port_fwding_private_ip  = 0xC0A80001;//192.168.0.1
            t_nat_info.port_fwding_private_port= 0x1234;
            t_nat_info.port_fwding_global_port = 0x4321;
            t_nat_info.port_fwding_protocol    = 123;
            
            ret = QL_MCM_MobileAP_DeleteStaticNATEntry(h_mobap, &t_nat_info);
            printf("QL_MCM_MobileAP_DeleteStaticNATEntry ret = %d\n", ret);
            break;
        }
            
        case 13://"QL_MCM_MobileAP_SetNATTimeout"
        {
            ret = QL_MCM_MobileAP_SetNATTimeout(h_mobap,
                                                E_QL_MOBAP_NAT_TIMEOUT_TYPE_GENERIC,
                                                60);
            printf("QL_MCM_MobileAP_SetNATTimeout(Generic, 60s) ret = %d\n", ret);
            break;
        }
            
        case 14://"QL_MCM_MobileAP_GetNATTimeout"
        {
            int timeout = 0;
            ret = QL_MCM_MobileAP_GetNATTimeout(h_mobap,
                                                E_QL_MOBAP_NAT_TIMEOUT_TYPE_GENERIC,
                                                &timeout);
            printf("QL_MCM_MobileAP_GetNATTimeout(Generic) ret = %d, timeout=%d\n", ret, timeout);
            break;
        }
            
        case 15://"QL_MCM_MobileAP_SetNATType"
        {
            E_QL_MOBAP_NAT_TYPE_T e_nat_type = E_QL_MOBAP_NAT_TYPE_SYMMETRIC;
            ret = QL_MCM_MobileAP_SetNATType(h_mobap, e_nat_type);
            printf("QL_MCM_MobileAP_SetNATType(E_QL_MOBAP_NAT_TYPE_SYMMETRIC) ret = %d\n", ret);
            break;
        }
            
        case 16://"QL_MCM_MobileAP_GetNATType"
        {
            E_QL_MOBAP_NAT_TYPE_T e_nat_type;
            ret = QL_MCM_MobileAP_GetNATType(h_mobap, &e_nat_type);
            printf("QL_MCM_MobileAP_GetNATType ret = %d, e_nat_type=%d\n", ret, e_nat_type);
            break;
        }
            
        case 17://"QL_MCM_MobileAP_AddFirewallEntry"
        {
            QL_MOBAP_FIREWALL_ENTRY_INFO_T   t_fw_entry;

            memset(&t_fw_entry, 0, sizeof(QL_MOBAP_FIREWALL_ENTRY_INFO_T));

            //This is just a example. You should fix t_fw_entry here!
            t_fw_entry.ip_version = E_QL_MOBILEAP_IP_TYPE_V4;
            
            ret = QL_MCM_MobileAP_AddFirewallEntry(h_mobap, &t_fw_entry);
            printf("QL_MCM_MobileAP_AddFirewallEntry ret = %d\n", ret);
            break;
        }
            
        case 18://"QL_MCM_MobileAP_DeleteFirewallEntry"
        {
            ret = QL_MCM_MobileAP_DeleteFirewallEntry(h_mobap, E_QL_MOBILEAP_IP_TYPE_V4, 1);
            printf("QL_MCM_MobileAP_DeleteFirewallEntry(IPV4, #1) ret = %d\n", ret);
            break;
        }
            
        case 19://"QL_MCM_MobileAP_GetFirewallEntry"
        {
            QL_MOBAP_FIREWALL_ENTRY_INFO_T  t_fw_entry;
            memset(&t_fw_entry, 0, sizeof(QL_MOBAP_FIREWALL_ENTRY_INFO_T));
            ret = QL_MCM_MobileAP_GetFirewallEntry(h_mobap,
                                                   E_QL_MOBILEAP_IP_TYPE_V4,
                                                   1, // 1 base
                                                   &t_fw_entry);
            printf("QL_MCM_MobileAP_GetFirewallEntry ret = %d, FirewallEntry info:...\n", ret);
            break;
        }
            
        case 20://"QL_MCM_MobileAP_SetFirewall"
        {
            uint8_t enable_firewall = 0;
            uint8_t pkts_allowed    = 0;
            
            printf("please input enable_firewall flag(0: disable, 1:enable): ");
            scanf("%d", &enable_firewall);
            
            printf("please input pkts_allowed flag(0: disable, 1:enable): ");
            scanf("%d", &pkts_allowed);
            
            ret = QL_MCM_MobileAP_SetFirewall(h_mobap,
                                              enable_firewall,
                                              pkts_allowed);
            printf("QL_MCM_MobileAP_SetFirewall ret = %d\n", ret);
            break;
        }
            
        case 21://"QL_MCM_MobileAP_AddDMZ"
        {
            ret = QL_MCM_MobileAP_AddDMZ(h_mobap, 0xC0A80101);//You should fill this with right value
            printf("QL_MCM_MobileAP_AddDMZ ret = %d\n", ret);
            break;
        }        
            
        case 22://"QL_MCM_MobileAP_DeleteDMZ"
            ret = QL_MCM_MobileAP_DeleteDMZ(h_mobap, 0xC0A80101);//You should fill this with right value
            printf("QL_MCM_MobileAP_DeleteDMZ ret = %d\n", ret);
            break;
            
        case 23://"QL_MCM_MobileAP_GetDMZ"
        {
            uint32_t dmz_ip_addr;
            struct in_addr  addr;
            
            ret = QL_MCM_MobileAP_GetDMZ(h_mobap, &dmz_ip_addr);
            addr.s_addr = ntohl(dmz_ip_addr);
            printf("QL_MCM_MobileAP_GetDMZ ret = %d, dmz_ip_addr: %s\n", ret, inet_ntoa(addr));            
            break;
        }
            
        case 24://"QL_MCM_MobileAP_GetIPv4WWANCfg"
        {
            QL_MOBAP_IPV4_WWAN_CONFIG_INFO_T    t_wwan_cfg = {0};
            ret = QL_MCM_MobileAP_GetIPv4WWANCfg(h_mobap, &t_wwan_cfg);
            printf("QL_MCM_MobileAP_GetIPv4WWANCfg ret = %d, IPV4 Config Info:...\n", ret);            
            break;
        }
            
        case 25://"QL_MCM_MobileAP_GetWWANStatistics"
        {
            QL_MOBAP_WWAN_STATISTICS_INFO_T t_info = {0};
            ret = QL_MCM_MobileAP_GetWWANStatistics(h_mobap,
                                                    E_QL_MOBILEAP_IP_TYPE_V4,
                                                    &t_info);
            printf("QL_MCM_MobileAP_GetWWANStatistics ret = %d, STATISTICS_INFO: ...\n", ret);
            break;
        }
            
        case 26://"QL_MCM_MobileAP_ResetWWANStatistics"
            ret = QL_MCM_MobileAP_ResetWWANStatistics(h_mobap, E_QL_MOBILEAP_IP_TYPE_V4);
            printf("QL_MCM_MobileAP_ResetWWANStatistics ret = %d\n", ret);
            break;
            
        case 27://"QL_MCM_MobileAP_SetDHCPDConfig"
        {
            QL_MOBAP_DHCP_CFG_INFO_T    t_info = {0};

            t_info.intf = 0;
            t_info.start= 0xC0A86464; //192.168.100.100  //just example, you can modify it.
            t_info.end  = 0xC0A8646E; //192.168.100.110
            strcpy(t_info.leasetime, "1h2m");
                       
            ret = QL_MCM_MobileAP_SetDHCPDConfig(h_mobap, &t_info);
            printf("QL_MCM_MobileAP_SetDHCPDConfig ret = %d, DHCP info:...\n", ret);
            break;
        }
            
        case 28://"QL_MCM_MobileAP_EnableWLAN"
            ret = QL_MCM_MobileAP_EnableWLAN(h_mobap);
            printf("QL_MCM_MobileAP_EnableWLAN ret = %d\n", ret);
            break;
            
        case 29://"QL_MCM_MobileAP_DisableWLAN"
            ret = QL_MCM_MobileAP_DisableWLAN(h_mobap);
            printf("QL_MCM_MobileAP_DisableWLAN ret = %d, \n", ret);
            break;
            
        case 30://"QL_MCM_MobileAP_SetIPSecVPNPassthrough"
            ret = QL_MCM_MobileAP_SetIPSecVPNPassthrough(h_mobap, 1);
            printf("QL_MCM_MobileAP_SetIPSecVPNPassthrough(Enable) ret = %d\n", ret);
            break;
            
        case 31://"QL_MCM_MobileAP_SetPPTPVPNPassthrough"
            ret = QL_MCM_MobileAP_SetPPTPVPNPassthrough(h_mobap, 1);
            printf("QL_MCM_MobileAP_SetPPTPVPNPassthrough ret = %d\n", ret);
            break;

        case 32://"QL_MCM_MobileAP_SetL2TPVPNPassthrough"
            ret = QL_MCM_MobileAP_SetL2TPVPNPassthrough(h_mobap, 1);
            printf("QL_MCM_MobileAP_SetL2TPVPNPassthrough ret = %d\n", ret);
            break;
            
        case 33://"QL_MCM_MobileAP_GetIPSecVPNPassthrough"
        {
            uint8_t enable = 0;
            ret = QL_MCM_MobileAP_GetIPSecVPNPassthrough(h_mobap, &enable);
            printf("QL_MCM_MobileAP_GetIPSecVPNPassthrough ret = %d, enable=%d\n", ret, enable);
            break;
        }
            
        case 34://"QL_MCM_MobileAP_GetPPTPVPNPassthrough"
        {
            uint8_t enable = 0;
            ret = QL_MCM_MobileAP_GetPPTPVPNPassthrough(h_mobap, &enable);
            printf("QL_MCM_MobileAP_GetPPTPVPNPassthrough ret = %d, enable=%d\n", ret, enable);
            break;
        }
            
        case 35://"QL_MCM_MobileAP_GetL2TPVPNPassthrough"
        {
            uint8_t enable = 0;
            ret = QL_MCM_MobileAP_GetL2TPVPNPassthrough(h_mobap, &enable);
            printf("QL_MCM_MobileAP_GetL2TPVPNPassthrough ret = %d, enable=%d\n", ret, enable);
            break;
        }
            
        case 36://"QL_MCM_MobileAP_SetRoaming"
            ret = QL_MCM_MobileAP_SetRoaming(h_mobap, 1);
            printf("QL_MCM_MobileAP_SetRoaming(Enable) ret = %d\n", ret);
            break;
            
        case 37://"QL_MCM_MobileAP_GetRoaming"
        {
            uint8_t enable = 0;
            ret = QL_MCM_MobileAP_GetRoaming(h_mobap, &enable);
            printf("QL_MCM_MobileAP_GetRoaming ret = %d, enable=%d\n", ret, enable);
            break;
        }
            
        case 38://"QL_MCM_MobileAP_SetDUALAPConfig"
            ret = QL_MCM_MobileAP_SetDUALAPConfig(h_mobap);
            printf("QL_MCM_MobileAP_SetDUALAPConfig ret = %d\n", ret);
            break;
            
        case 39://"QL_MCM_MobileAP_EnableSTAMode"
        {
            QL_MOBAP_STA_CONNECTION_CFG_INFO_T  t_info = {0};

            t_info.e_conn_type = E_QL_MOBAP_STA_CONNECTION_TYPE_DYNAMIC;
            ret = QL_MCM_MobileAP_EnableSTAMode(h_mobap,
                                                1, //0: means drop pt_info, 1:use pt_info setting
                                                &t_info);
            printf("QL_MCM_MobileAP_EnableSTAMode ret = %d\n", ret);
            break;
        }
            
        case 40://"QL_MCM_MobileAP_DisableSTAMode"
            ret = QL_MCM_MobileAP_DisableSTAMode(h_mobap);
            printf("QL_MCM_MobileAP_DisableSTAMode ret = %d\n", ret);
            break;

        default:
            show_group_help(&t_mobap_test);
        }
    }
    return 0;
}


func_api_test_t t_mobap_test = {"mcm_mobap", at_mobap_testlist, test_mobap};

