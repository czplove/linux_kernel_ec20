#include <ql_oe.h>

#define BUF_SIZE            128
#define SUPPORTED_APN_MAX   5

#define QL_WWAN_LOG(format, ...) fprintf(stderr, "[QL_CLI][%d]:"format, __LINE__, ##__VA_ARGS__) 
    
static int g_cur_apn_idx = 0;
static dsi_hndl_t g_h_handle[SUPPORTED_APN_MAX] = {0};
static int g_is_network_ready[SUPPORTED_APN_MAX]= {0};
static unsigned short route_flag = 0;    // default route flag
 

void nw_init_cb(void* cb_data)
{
    QL_WWAN_LOG("nw_init_cb!\n");    
}

void dsi_net_evt_cb( dsi_hndl_t         hndl,         /**< Handle for which this event is associated. */
                    void              *user_data,    /**< Application-provided user data. */
                    dsi_net_evt_t      evt,          /**< Event identifier. */
                    dsi_evt_payload_t *payload_ptr   /**< Associated event information. */
                  )
{
    int apn_idx = (int)user_data;
    
    QL_WWAN_LOG("APN[%d] dsi_net_evt_cb, evt=%d!\n", apn_idx, evt);  
    switch(evt)
    {
        case DSI_EVT_NET_IS_CONN:
            QL_WWAN_LOG("Call is connected!!\n"); 
            break;
        case DSI_EVT_NET_NO_NET: /**< Call is disconnected. */
            QL_WWAN_LOG("Call is Disconnected!!\n"); 
            g_is_network_ready[apn_idx] = 0;
            break;
        case DSI_EVT_NET_NEWADDR:
            QL_WWAN_LOG("New address is generated!!\n"); 
            break;
        case DSI_EVT_NET_DELADDR:
            QL_WWAN_LOG("Old address deleted !!\n"); 
            break;
        case DSI_EVT_WDS_CONNECTED:
            QL_WWAN_LOG("Network Ready!!\n"); 
            g_is_network_ready[apn_idx] = 1;
            break;
        default:
            break;
    }
}

//"QL_WWAN_StartDataCall"
bool start_datacall()
{
    	    int    ret = E_QL_OK;
            char   command[BUF_SIZE]= {0};
            char   buf[BUF_SIZE] = {0};
            char   ip_str[64]  = {0};
            int    valid_ip_cnt= 0;
            dsi_addr_info_t info;
	    int wait_time = 10;
            
            ret = QL_WWAN_StartDataCall(g_h_handle[g_cur_apn_idx]);
            QL_WWAN_LOG("QL_WWAN_StartDataCall ret=%d!\n", ret);
            while((g_is_network_ready[g_cur_apn_idx] == 0) && (wait_time-- > 0 ))
            {
                QL_WWAN_LOG("Wait for network ready!\n");
                sleep(1);
            }
            
            if(g_is_network_ready[g_cur_apn_idx] == 0)
	    {
                QL_WWAN_LOG("Check your network please!\n");
		return FALSE;
	    }

            ret = QL_WWAN_GetIPAddr(g_h_handle[g_cur_apn_idx], &info, &valid_ip_cnt);
            if(info.iface_addr_s.valid_addr == 0)
            {
                QL_WWAN_LOG("QL_WWAN_GetIPAddr ret = %d, NO valid addr!!\n", ret);
                return FALSE;
            }            
            
            inet_ntop(AF_INET, info.iface_addr_s.addr.__ss_padding, ip_str, sizeof(ip_str));
            QL_WWAN_LOG("QL_WWAN_GetIPAddr ret = %d, valid_ip_cnt=%d; Interface IP address =%s\n",  ret, valid_ip_cnt, ip_str);
            
            ret = QL_WWAN_GetDeviceName(g_h_handle[g_cur_apn_idx], buf, sizeof(buf));
            QL_WWAN_LOG("QL_WWAN_GetDeviceName ret = %d, name=%s\n", ret, buf);

            snprintf(command, sizeof(command), "ifconfig %s %s netmask 255.0.0.0",buf, ip_str);
            system(command);
           
            /* remove the default route rule, otherwise we cannot add the new one, by gale */
            if(route_flag)
            {
                memset(command, 0, sizeof(command));
                snprintf(command, sizeof(command), "ip route del default");
                system(command);
            }
            inet_ntop(AF_INET, &info.gtwy_addr_s.addr.__ss_padding, ip_str, sizeof(ip_str));
            snprintf(command, sizeof(command), "ip route add default via %s dev %s", ip_str, buf);
            system(command);
            route_flag = 1;    //Mark: there is already a defaule route rule
            QL_WWAN_LOG("Gateway IP address   =%s\n", ip_str);

            inet_ntop(AF_INET, &info.dnsp_addr_s.addr.__ss_padding, ip_str, sizeof(ip_str));
            snprintf(command, sizeof(command), "echo 'nameserver %s' > /etc/resolv.conf", ip_str);
            system(command);
            QL_WWAN_LOG("Primary DNS address  =%s\n", ip_str);
            
            inet_ntop(AF_INET, &info.dnss_addr_s.addr.__ss_padding, ip_str, sizeof(ip_str));
            snprintf(command, sizeof(command), "echo 'nameserver %s' >> /etc/resolv.conf", ip_str);
            system(command);
            QL_WWAN_LOG("Secondary DNS address=%s\n", ip_str);
            return TRUE;
}

int main()
{
    int ret = E_QL_OK;

    if(daemon(1, 1) < 0)  
    {  
        perror("daemon");  
        exit(1);  
    } 

    //"QL_WWAN_Initialize"
    ret = QL_WWAN_Initialize(nw_init_cb, NULL);
    QL_WWAN_LOG("QL_WWAN_Initialize ret=%d!\n", ret);
    sleep(2);	//wait init ready

    for(;g_cur_apn_idx < 2; g_cur_apn_idx++)
    {

    	//"QL_WWAN_GetDataSrvHandle"
	g_h_handle[g_cur_apn_idx] = QL_WWAN_GetDataSrvHandle(dsi_net_evt_cb, (void*)g_cur_apn_idx);
	QL_WWAN_LOG("QL_WWAN_GetDataSrvHandle g_h_handle=%d!\n", (int)g_h_handle[g_cur_apn_idx]);

	//"QL_WWAN_SetIPVersion"
        ret = QL_WWAN_SetProfileId(g_h_handle[g_cur_apn_idx], g_cur_apn_idx+1);
	QL_WWAN_LOG("QL_WWAN_SetProfileId ret = %d\n", ret);
	ret = QL_WWAN_SetIPVersion(g_h_handle[g_cur_apn_idx], 4);
	QL_WWAN_LOG("QL_WWAN_SetIPVersion ret = %d\n", ret);

	if(start_datacall() == FALSE)
	{
	    QL_WWAN_LOG("Start datacall failed\n");
	    exit(1);
	}
	QL_WWAN_LOG("[APN%d]: OK\n\n", g_cur_apn_idx);
    }

    while(1)sleep(1);

    //"QL_WWAN_StopDataCall"
    //ret = QL_WWAN_StopDataCall(g_h_handle[g_cur_apn_idx]);
    //QL_WWAN_LOG("QL_WWAN_StopDataCall ret=%d!\n", ret);

    return 0;
}
