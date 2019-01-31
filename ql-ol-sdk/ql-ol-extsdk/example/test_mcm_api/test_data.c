#include <ql_oe.h>
#include "test_base.h"

extern func_api_test_t t_data_test;

static data_client_handle_type  h_data = 0;
st_api_test_case at_data_testlist[] = 
{
    {0,     "QL_MCM_DATA_Client_Init"},
    {1,     "QL_MCM_DATA_AddRxIndMsgHandler"},
    {2,     "QL_MCM_DATA_SetIPFamily"},
    {3,     "QL_MCM_DATA_SetTechPref"},
    {4,     "QL_MCM_DATA_SetProfile"},
    {5,     "QL_MCM_DATA_SetAPN"},
    {6,     "QL_MCM_DATA_SetUsername"},
    {7,     "QL_MCM_DATA_SetPassword"},
    {8,     "QL_MCM_DATA_StartDataCall"},
    {9,     "QL_MCM_DATA_StopDataCall"},
    {10,    "QL_MCM_DATA_GetDataCallStatus"},
    {11,    "QL_MCM_DATA_GetDataCallBearerTech"},
    {12,    "QL_MCM_DATA_GetDeviceName"},
    {13,    "QL_MCM_DATA_GetDeviceAddress"},
    {14,    "QL_MCM_DATA_GetDeviceAddrCount"},
    {15,    "QL_MCM_DATA_GetPktStatistic"},
    {16,    "QL_MCM_DATA_ResetPktStatistic"},
    {17,    "QL_MCM_DATA_GetRegStatus"},
    {18,    "QL_MCM_DATA_GetDataChannelRate"},
    {19,    "QL_MCM_DATA_AddRoute"},
    {20,    "QL_MCM_DATA_RegisterEvent"},
    {21,    "QL_MCM_DATA_Client_Deinit"},
    {22,    "Create new DataCall"},
    
    {-1,    NULL}
};

char *addr2str(QL_MCM_DATA_CALL_ADDR_T *addr, char *buf, int len, int *pIsIpv6)
{
    char *ptr;
    int i;
    int isIpv6 = 0;

    if(addr->valid_addr==0) {
        return NULL;
    }
    
    if(addr->addr.ss_family == AF_INET6) {
        isIpv6 = 1;
    }

    if(pIsIpv6) {
        pIsIpv6[0] = isIpv6;
    }
    if(isIpv6==0) {
        snprintf(buf, len, "%d.%d.%d.%d", addr->addr.__ss_padding[0], 
                addr->addr.__ss_padding[1],addr->addr.__ss_padding[2],addr->addr.__ss_padding[3]);
    }
    else {
        inet_ntop(AF_INET6, &(addr->addr), buf, sizeof(addr->addr));
    }

    return buf;
}

static void ql_mcm_data_nfy
(
    data_client_handle_type         h_data,
    E_QL_DATA_NET_EVENT_MSG_ID_T    e_msg_id,
    QL_MCM_DATA_CALL_INDICATION_INFO_T *data,
    void                            *contextPtr    
)
{
    int is_ipv6 = 0;
    char *event[] = {   "unknown", 
                        "NET_UP", 
                        "NET_DOWN", 
                        "NEW_ADDR", 
                        "DEL_ADDR", 
                        "SERVICE_STATUS", 
                        "BEARER_TECH_STATUS",
                        "DORMANCY_STATUS"};
    char *call_status[] = {   "INVALID", 
                        "INVALID", 
                        "CONNECTING",
                        "CONNECTED",
                        "DISCONNECTING",
                        "DISCONNECTED"};
    printf("### %s got %s event!\n", __func__, event[e_msg_id - 0x5000]);

    switch(e_msg_id)
    {
    case E_QL_DATA_NET_UP_EVENT:
        break;
    case E_QL_DATA_NET_DOWN_EVENT:
        break;
    case E_QL_DATA_NET_NEW_ADDR_EVENT:
        break;
    case E_QL_DATA_NET_DEL_ADDR_EVENT:
        break;
    case E_QL_DATA_NET_SERVICE_STATUS_EVENT:
        break;
    case E_QL_DATA_NET_BEARER_TECH_STATUS_EVENT:
        break;
    case E_QL_DATA_NET_DORMANCY_STATUS_EVENT:
        break;
    }
    
    if(data->call_id_valid) 
    {
        printf("call_id : %d\n", data->call_id);
    }
    if(data->call_status_valid) 
    {
        printf("call_status : %s\n", call_status[data->call_status]);
    }
    if(data->call_tech_valid) 
    {
        printf("call_tech : %d\n", data->call_tech);
    }
    if(data->reg_status_valid) 
    {
        printf("reg_status : %d\n", data->reg_status);
    }
    if(data->dorm_status_valid)
    {
        printf("dorm_status : %d\n", data->dorm_status);
    }
    if(data->addr_count_valid) 
    {
        printf("addr_count  : %d\n", data->addr_count);
    }
    if(data->addr_info_valid) 
    {
        int i;
        QL_MCM_DATA_CALL_ADDR_INFO_T *addr;
        char *ptr;
        char tmpBuf[246];
        struct in_addr ia;
        printf("addr_info_len : %d\n", data->addr_info_len);
        for(i=0; i<data->addr_info_len; i++) 
        {
            addr = &data->addr_info[i];
            ptr = addr2str(&addr->iface_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
            if(ptr) 
            {
                printf("ipaddr : %s/%d \n", ptr, addr->iface_mask);
            }

            ptr = addr2str(&addr->gtwy_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
            if(ptr) 
            {
                printf("gateway : %s/%d \n", ptr, addr->gtwy_mask);
            }

            ptr = addr2str(&addr->dnsp_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
            if(ptr) 
            {
                printf("Primary DNS : %s\n", ptr);
            }

            ptr = addr2str(&addr->dnss_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
            if(ptr) 
            {
                printf("Secondary DNS : %s\n", ptr);
            }
        }
    }
    if(data->vce_reason_valid) 
    {
        printf("data call end reason : %d\n", data->vce_reason);
    }
}


static int test_data(void)
{
    int     cmdIdx  = 0;
    E_QL_ERROR_CODE_T ret;
    QL_MCM_DATA_CALL_RESULT_INFO_T  t_call_result = {0};
    QL_MCM_DATA_CALL_CONFIG_INFO_T  t_cfg = {0};
        
    show_group_help(&t_data_test);

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
        case 0://"QL_MCM_DATA_Client_Init"
            ret = QL_MCM_DATA_Client_Init(&h_data);
            printf("QL_MCM_DATA_Client_Init ret = %d, h_data=0x%X\n", ret, h_data);
            break;

        case 1://"QL_MCM_DATA_AddRxIndMsgHandler"
            ret = QL_MCM_DATA_AddRxIndMsgHandler(ql_mcm_data_nfy, (void*)h_data);
            printf(" ret = %d\n", ret);
            break;
            
        case 2://"QL_MCM_DATA_SetIPFamily"
            {
                int tmp = 0;
                printf("please input ip family (4-ipv4 6-ipv6 10-ipv4&ipv6 other-skip): ");
                scanf("%d", &tmp);
                if(tmp==4) {
                    printf("IP Family : IPV4\n");
                }
                else if(tmp==6) {
                    printf("IP Family : IPV6\n");
                }
                else if(tmp==10) {
                    printf("IP Family : IPV4 & IPV6\n");
                }
                else {
                    printf("skip\n");
                    break;
                }
                ret = QL_MCM_DATA_SetIPFamily(&t_cfg, tmp, TRUE);
                printf("QL_MCM_DATA_SetIPFamily ret = %d\n", ret);
                break;
            }
            
        case 3://"QL_MCM_DATA_SetTechPref"
            {
                int tech = E_QL_MCM_DATA_TECH_PREF_AUTO;
                printf("please input TechPref (1-CDMA 2-UMTS 7-auto other-skip): ");
                scanf("%d", &tech);
                if(tech==1) {
                    printf("TechPref : CMDA\n");
                }
                else if(tech==2) {
                    printf("TechPref : UMTS\n");
                }
                else if(tech==7) {
                    printf("TechPref : AUTO\n");
                }
                else {
                    printf("skip\n");
                    break;
                }

                ret = QL_MCM_DATA_SetTechPref(&t_cfg, tech, TRUE);
                printf("QL_MCM_DATA_SetTechPref ret = %d\n", ret);
                break;
            }
            
        case 4://"QL_MCM_DATA_SetProfile"
        {
            int profile_id;
            printf("please input profile ID: ");
            scanf("%d", &profile_id);
            ret = QL_MCM_DATA_SetProfile(&t_cfg, profile_id, TRUE);
            printf("QL_MCM_DATA_SetProfile ret = %d\n", ret);
            break;
        }
            
        case 5://"QL_MCM_DATA_SetAPN"
        {
            char apn[32];
            printf("please input APN name: ");
            scanf("%s", apn);
            ret = QL_MCM_DATA_SetAPN(&t_cfg, apn, TRUE);
            printf("QL_MCM_DATA_SetAPN ret = %d\n", ret);
            break;
        }
            
        case 6://"QL_MCM_DATA_SetUsername"
        {
            char username[32];
            printf("please input user name: ");
            scanf("%s", username);
            ret = QL_MCM_DATA_SetUsername(&t_cfg, username, TRUE);
            printf("QL_MCM_DATA_SetUsername ret = %d\n", ret);
            break;
        }
            
        case 7://"QL_MCM_DATA_SetPassword"
        {
            char password[32];
            printf("please input password: ");
            scanf("%s", password);
            ret = QL_MCM_DATA_SetPassword(&t_cfg, password, TRUE);
            printf("QL_MCM_DATA_SetPassword ret = %d\n", ret);
            break;
        }
            
        case 8://"QL_MCM_DATA_StartDataCall"
        {
            ret = QL_MCM_DATA_StartDataCall(h_data, &t_cfg, &t_call_result);
            printf("QL_MCM_DATA_StartDataCall ret = %d, callid=%d \n", ret, t_call_result.call_id);
            break;
        }
            
        case 9://"QL_MCM_DATA_StopDataCall"
        {
            int callid = -1;
            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_StopDataCall(h_data, callid);
            printf("QL_MCM_DATA_StopDataCall ret = %d\n", ret);
            break;
        }
            
        case 10://"QL_MCM_DATA_GetDataCallStatus"
        {
            E_QL_MCM_DATA_CALL_STATE_TYPE_T e_status;
            int callid = -1;
            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_GetDataCallStatus(h_data, callid, &e_status);
            printf("QL_MCM_DATA_GetDataCallStatus ret = %d, e_status=%d\n", ret, e_status);
            break;
        }
            
        case 11://"QL_MCM_DATA_GetDataCallBearerTech"
        {
            E_QL_MCM_DATA_BEARER_TECH_TYPE_T    e_tech;
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_GetDataCallBearerTech(h_data, callid, &e_tech);
            printf("QL_MCM_DATA_GetDataCallBearerTech ret = %d, e_tech= %d\n", ret, e_tech);
            break;
        }
            
        case 12://"QL_MCM_DATA_GetDeviceName"
        {
            char dev_name[13] = {0};
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }
            ret = QL_MCM_DATA_GetDeviceName(h_data, callid, dev_name, 13);
            printf("QL_MCM_DATA_GetDeviceName ret = %d with dev_name=%s\n", ret, dev_name);
            break;
        }
            
        case 13://"QL_MCM_DATA_GetDeviceAddress"
        {
            int i;
            char tmpBuf[256];
            char *ptr;
            int is_ipv6 = 0;
            QL_MCM_DATA_CALL_ADDR_INFO_LIST_T   t_addr_list = {0};
            QL_MCM_DATA_CALL_ADDR_INFO_T *addr;
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_GetDeviceAddress(h_data, callid, &t_addr_list);
            printf("QL_MCM_DATA_GetDeviceAddress ret = %d, valid addr count=%d, details.....\n", ret, t_addr_list.addr_info_len);
            for(i=0; i<t_addr_list.addr_info_len; i++) {
                addr = &(t_addr_list.addr_info[i]);

                ptr = addr2str(&addr->iface_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("ipaddr : %s/%d \n", ptr, addr->iface_mask);
                }
                else {
                    printf("Failed to parse addr\n");
                }

                ptr = addr2str(&addr->gtwy_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("gateway : %s/%d \n", ptr, addr->gtwy_mask);
                }

                ptr = addr2str(&addr->dnsp_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("Primary DNS : %s\n", ptr);
                }

                ptr = addr2str(&addr->dnss_addr_s, tmpBuf, sizeof(tmpBuf), &is_ipv6);
                if(ptr) {
                    printf("Secondary DNS : %s\n", ptr);
                }

            }
            break;
        }
            
        case 14://"QL_MCM_DATA_GetDeviceAddrCount"
        {
            int count = 0;
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_GetDeviceAddrCount(h_data, callid, &count);
            printf("QL_MCM_DATA_GetDeviceAddrCount ret = %d, count=%d\n", ret, count);
            break;
        }
            
        case 15://"QL_MCM_DATA_GetPktStatistic"
        {
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            QL_MCM_DATA_PKT_STATISTIC_INFO_T    t_info = {0};
            ret = QL_MCM_DATA_GetPktStatistic(h_data, callid, &t_info);
            printf("QL_MCM_DATA_GetPktStatistic ret = %d, bytes_rx=%lld, bytes_tx=%llx\n", 
                    ret, t_info.bytes_rx, t_info.bytes_tx);
            break;
        }
            
        case 16://"QL_MCM_DATA_ResetPktStatistic"
        {
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_ResetPktStatistic(h_data, callid);
            printf("QL_MCM_DATA_ResetPktStatistic ret = %d\n", ret);
            break;
        }
        case 17://"QL_MCM_DATA_GetRegStatus"
        {;
            QL_MCM_DATA_REG_STATUS_INFO_T       t_info = {0};
            ret = QL_MCM_DATA_GetRegStatus(h_data, &t_info);
            printf("QL_MCM_DATA_GetRegStatus ret = %d, modem_state=%d, bearer_tech\n", 
                    ret, t_info.srv_status, t_info.tech_info);
            break;
        }
        case 18://"QL_MCM_DATA_GetDataChannelRate"
        {
            QL_MCM_DATA_CHANNEL_RATE_INFO_T     t_info;
            int callid = -1;

            printf("Please input call_id :\n");
            scanf("%d", &callid);
            if(callid<=0) {
                printf("Invalid call id, skip\n");
                break;
            }

            ret = QL_MCM_DATA_GetDataChannelRate(h_data, callid, &t_info);
            printf("QL_MCM_DATA_GetDataChannelRate ret = %d, "
                    "cur_tx_rate=%d, cur_rx_rate=%d, max_tx_rate=%d, max_rx_rate=%d\n", 
                    ret, t_info.current_tx_rate, t_info.current_rx_rate, t_info.max_tx_rate, t_info.max_rx_rate);
            break;
        }
        case 19://"QL_MCM_DATA_AddRoute"
        {
            char buf[256];
            QL_MCM_DATA_ROUTE_INFO_T            t_info = {0};

            printf("please input dest ip address: \n");
            scanf("%32s", t_info.ip_addr); // won't accept empty string
            if(inet_addr(t_info.ip_addr)==0 || inet_addr(t_info.ip_addr)==0xFF) {
                printf("Invalid ip format : %s\n", t_info.ip_addr); 
                break;
            }

            printf("please input gateway(default is null): \n");
            scanf("%32s", t_info.gateway);
            if(inet_addr(t_info.gateway)==0 || inet_addr(t_info.gateway)==0xFF) {
                printf("Invalid ip format : %s\n", t_info.gateway); 
                break;
            }


            printf("please input valid iface_name: \n");
            scanf("%40s", t_info.iface_name); // won't accept empty string
            
            ret = QL_MCM_DATA_AddRoute(h_data, &t_info);
            printf("QL_MCM_DATA_AddRoute ret = %d\n", ret);
            break;
        }
        case 20://"QL_MCM_DATA_RegisterEvent"
        {
            int mask = 0;
            printf("please input event mask(HEX base)(DORMANCY | BEARER_TECH | SERVICE | DEL_ADD | NEW_ADDR | DOWN | UP): \n");
            scanf("%x", &mask);
            ret = QL_MCM_DATA_RegisterEvent(h_data, mask);            
            printf("QL_MCM_DATA_RegisterEvent ret = %d\n", ret);
            break;
        }
        case 21://"QL_MCM_DATA_Client_Deinit"
            ret = QL_MCM_DATA_Client_Deinit(h_data);
            printf("QL_MCM_DATA_Client_Deinit ret = %d\n", ret);
            break;
            
        case 22:
            printf("Create new data call channel\n");
            memset(&t_cfg, 0, sizeof(t_cfg));
            break;
        default:
            show_group_help(&t_data_test);
        }
    }
    return 0;
}


func_api_test_t t_data_test = {"mcm_data", at_data_testlist, test_data};

