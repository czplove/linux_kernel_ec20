#include <ql_oe.h>

#define QUEC_AT_PORT    "/dev/smd8"
#define BUF_SIZE        128
#define SUPPORTED_APN_MAX           5

#define TEST_PORT                   5678    // the port users will be connecting to
#define SUPPORTED_CONNECTION_MAX    1       // how many pending connections queue will hold
 

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;

static sms_client_handle_type      h_sms;
    
static sem_t binSem;
static int g_cur_apn_idx = 0;
static dsi_hndl_t g_h_handle[SUPPORTED_APN_MAX] = {0};
static int g_is_network_ready[SUPPORTED_APN_MAX]= {0};

//for server test
static int g_accept_fd[SUPPORTED_CONNECTION_MAX];         // accepted connection fd
static int g_cur_conn_cnt; 
static int g_is_server_daemon_running = 0; 

#define ATC_REQ_CMD_MAX_LEN     513
#define ATC_RESP_CMD_MAX_LEN    4097
static char atc_cmd_req[ATC_REQ_CMD_MAX_LEN]    = {0};
static char atc_cmd_resp[ATC_RESP_CMD_MAX_LEN]  = {0};

st_api_test_case at_api_testcases[] = 
{
    {0,    "QL_DEV_GetImei"},
    {1,    "QL_DEV_GetFirmwareVersion"},
    {2,    "QL_DEV_GetDeviceModel"},
    {3,    "QL_DEV_GetMeid"},
    {4,    "QL_DEV_GetEsn\n"},
    
    {40,    "QL_LOC Init"},
    {41,    "QL_LOC_Start_Navigation"},
    {42,    "QL_LOC_Stop_Navigation"},
    {43,    "QL_LOC_Get_Current_Location"},
    {44,    "QL_LOC_Client_Deinit"},
    {45,    "QL_LOC_Xtra_InjectFile\n"},
    
    {50,    "QL_ATC_Client_Init"},
    {51,    "QL_ATC_Send_Cmd"},
    {52,    "QL_ATC_Client_Deinit\n"},

    {60,    "QL_SMS_Client_Init"},
    {61,    "QL_SMS_Send_Sms"},
    {62,    "QL_SMS_Client_Deinit"},
    {63,    "QL_SMS_AddRxMsgHandler"},
    {64,    "QL_SMS_SetRouteList (Parse sms without store by default, if store, it won't parse)"},
    {65,    "QL_SMS_DeleteFromStorage\n"},

    {80,    "set_selected_apn_idx"},
    {90,    "test_sleep_wakeup"},
    
    {97,    "testapi_client(work as client, send msg to server)"},
    {98,    "stop testapi_client testing"},
    
    {99,    "testapi_server(work as server, receive msg and ack msg+ack)"},
    {100,   "stop testapi_server testing"},

    
    {101,   "test_udp_server"},
    {102,   "stop test_udp_server testing"},
    
    {103,   "test_udp_client"},
    
    {104,   "testapi_client_send_ack"},
    
    {110,   "start_qmi_timer to send heartbeat"},
    {111,   "auto sleep onoff"},
    
    
    {112,   "start_qmi_timer to send sms"},

    {120,   "resolve private domain name"},

    {130,   "forbid indication"},
    {132,   "register gpio cb handler,(you must add the quectel-low-consume driver into kernel firstly)\n"},
    
    {150,   "QL_Voice_Call_Client_Init"},
    {151,   "QL_Voice_Call_Client_Deinit"},
    {152,   "QL_Voice_Call_AddStateHandler"},
    {153,   "QL_Voice_Call_RemoveStateHandler"},
    {154,   "QL_Voice_Call_Start"},
    {155,   "QL_Voice_Call_End"},
    {156,   "QL_Voice_Call_Answer"},
    {157,   "QL_Voice_Call_Hold\n"},

    {158,   "Ql_Set_FirmwareRestore"},
    {159,   "Ql_Get_RestoreCnt"},

    {-1,    NULL}
};

void print_help(void)
{
    int i;

    printf("Supported test cases:\n");
    for(i = 0; ; i++)
    {
        if(at_api_testcases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", at_api_testcases[i].cmdIdx, at_api_testcases[i].funcName);
    }
}



#define MAX_CHARACTER_SIZE    8   
unsigned char* UnicodeToUTF8( int unicode, unsigned char *p)  
{  
    unsigned char *e = NULL;  
    if((e = p))  
    {  
        if(unicode < 0x80)  
        {  
            *e++ = unicode;  
        }  
        else if(unicode < 0x800)  
        {
            *e++ = ((unicode >> 6) & 0x1f)|0xc0;  
            *e++ = (unicode & 0x3f)|0x80;   
        }  
        else if(unicode < 0x10000)  
        {
            *e++ = ((unicode >> 12) & 0x0f)|0xe0;   
            *e++ = ((unicode >> 6) & 0x3f)|0x80;  
            *e++ = (unicode & 0x3f)|0x80;   
        }  
        else if(unicode < 0x200000)  
        {
            *e++ = ((unicode >> 18) & 0x07)|0xf0;   
            *e++ = ((unicode >> 12) & 0x3f)|0x80;  
            *e++ = ((unicode >> 6) & 0x3f)|0x80;  
            *e++ = (unicode & 0x3f)|0x80;   
        }  
        else if(unicode < 0x4000000)  
        {
            *e++ = ((unicode >> 24) & 0x03)|0xf8 ;   
            *e++ = ((unicode >> 18) & 0x3f)|0x80;  
            *e++ = ((unicode >> 12) & 0x3f)|0x80;  
            *e++ = ((unicode >> 6) & 0x3f)|0x80;  
            *e++ = (unicode & 0x3f)|0x80;   
        }  
        else 
        {
            *e++ = ((unicode >> 30) & 0x01)|0xfc;
            *e++ = ((unicode >> 24) & 0x3f)|0x80;
            *e++ = ((unicode >> 18) & 0x3f)|0x80;
            *e++ = ((unicode >> 12) & 0x3f)|0x80;
            *e++ = ((unicode >> 6) & 0x3f)|0x80;  
            *e++ = (unicode & 0x3f)|0x80;   
        }  
    }  
     
    return e;  
}  
 
int UTF8ToUnicode (unsigned char *ch, int *unicode)  
{  
    unsigned char *p = NULL;  
    int e = 0, n = 0;  
    if((p = ch) && unicode)  
    {  
        if(*p >= 0xfc)  
        {
            e = (p[0] & 0x01) << 30;  
            e |= (p[1] & 0x3f) << 24;  
            e |= (p[2] & 0x3f) << 18;  
            e |= (p[3] & 0x3f) << 12;  
            e |= (p[4] & 0x3f) << 6;  
            e |= (p[5] & 0x3f);  
            n = 6;  
        }  
        else if(*p >= 0xf8)   
        {
            e = (p[0] & 0x03) << 24;  
            e |= (p[1] & 0x3f) << 18;  
            e |= (p[2] & 0x3f) << 12;  
            e |= (p[3] & 0x3f) << 6;  
            e |= (p[4] & 0x3f);  
            n = 5;  
        }  
        else if(*p >= 0xf0)  
        {
            e = (p[0] & 0x07) << 18;  
            e |= (p[1] & 0x3f) << 12;  
            e |= (p[2] & 0x3f) << 6;  
            e |= (p[3] & 0x3f);  
            n = 4;  
        }  
        else if(*p >= 0xe0)  
        {
            e = (p[0] & 0x0f) << 12;  
            e |= (p[1] & 0x3f) << 6;  
            e |= (p[2] & 0x3f);  
            n = 3;  
        }  
        else if(*p >= 0xc0)   
        {
            e = (p[0] & 0x1f) << 6;  
            e |= (p[1] & 0x3f);  
            n = 2;  
        }  
        else   
        {  
            e = p[0];  
            n = 1;  
        }  
        *unicode = e;  
    }  
     
    return n;  
}  
 
unsigned char* UnicodeStrToUTF8Str (unsigned short *unicode_str,  
                                    unsigned char   *utf8_str, 
                                    int             utf8_str_size)  
{  
    int unicode = 0;  
    unsigned char *e = NULL, *s = NULL;  
    unsigned char utf8_ch[MAX_CHARACTER_SIZE];   
    s = utf8_str;  
    if ((unicode_str) && (s))  
    {  
        while ((unicode = (int) (*unicode_str++)))  
        {  
            #if 1//LE
                unicode = ((unicode&0xFF)<<8) | ((unicode&0xFF00) >> 8);  //error
            #else//BE
                unicode = unicode;  
            #endif
            memset (utf8_ch, 0, sizeof (utf8_ch));  
              
            if ((e = UnicodeToUTF8 (unicode, utf8_ch)) > utf8_ch)  
            {  
                *e = '\0';
                 
                if ((s - utf8_str + strlen ((const char *) utf8_ch)) >= utf8_str_size)  
                {  
                    return s;  
                }  
                else 
                {  
                    memcpy (s, utf8_ch, strlen ((const char *) utf8_ch));  
                    s += strlen ((const char *) utf8_ch);  
                    *s = '\0';  
                }  
            }  
            else 
            {
                return s;  
            }  
        }  
    }  
      
    return s;  
}  
 
int UTF8StrToUnicodeStr(unsigned char   *utf8_str,  
                        uint16_t        *unicode_str, 
                        int             unicode_str_size)  
{  
    int unicode = 0;  
    int n = 0;  
    int count = 0;  
    unsigned char *s = NULL;  
    uint16_t *e = NULL;  
      
    s = utf8_str;  
    e = unicode_str;  
      
    if ((utf8_str) && (unicode_str))  
    {  
        while (*s)  
        {  
            if ((n = UTF8ToUnicode (s, &unicode)) > 0)  
            {  
                if ((count + 1) >= unicode_str_size)  
                {  
                    return count;  
                }  
                else 
                {  
                #if 0//LE
                    *e = (unsigned short) unicode;  
                #else//BE
                    *e = ((unicode&0xFF)<<8) | ((unicode&0xFF00) >> 8);  
                #endif
                    e++;  
                    *e = 0;                     
                    s += n;  
                    count++;
                }  
            }  
            else 
            {
                return count;  
            }  
        }  
    }  
      
    return count;  
} 


int start_qmi_timer(ql_timer_exp_cb exp_cb_fcn, int timeout_sec, int repeat, void* param)
{
    int timerID = 0;  
    
    while(timerID < 10)
    {//get available timer id.
        if(QL_Timer_Register(timerID, exp_cb_fcn, param) == 0)
        {
            break;
        }
        timerID++;
    }
    if(timerID == 10)
    {
        printf("No available timer ID found\n");
        return -1;
    }
    QL_Timer_Start(timerID, timeout_sec*1000, repeat!=0);
    
    return timerID;
}


static void ql_vcall_cb_func(ST_VCALL_CallRef    reference,
    const char* identifier,
                             E_QL_VCALL_STATE_T  state,
                             void*               contextPtr)
{
    ST_VCALL_CallRef *ph_call = (ST_VCALL_CallRef *)contextPtr;
    char *call_state[] = {  "Unknown",  "ORIGINATING",  "INCOMING", "CONVESATION",  "CC_IN_PROGRESS", 
                            "ALERTING", "HOLD",         "WAITING",  "DISCONNECTING","END", 
                            "SETUP"};
    if(*ph_call == 0)//Call_id got here, you can save it for the later operation
    {
        *ph_call = reference;
    }
    printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", reference, identifier, call_state[state]);
}

static void ql_voice_call_cb_func(int                     call_id,
                                  char*                   phone_num,
                                  E_QL_VOICE_CALL_STATE_T state,
                                  void                    *contextPtr)
{
    voice_client_handle_type *ph_voice = (voice_client_handle_type *)contextPtr;
    
    char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

    if(*ph_voice == 0)//Call_id got here, you can save it for the later operation
    {
        *ph_voice = call_id;
    }
    
    printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n", call_id, phone_num, call_state[state]);
}

static void ql_nw_regstate_cb_func( E_QL_WWAN_NET_REG_STATE_T   state,
                                    void                        *contextPtr)
{
    printf("######### Current network regstate changed to %d  ######\n", state);
}

static void ql_sms_cb_func( QL_SMS_MsgRef   msgRef,  
                            void*               contextPtr)
{
    int i;
    if(msgRef->e_storage != E_QL_SMS_STORAGE_TYPE_NONE)
    {
        char *msg_format[]  = {"CDMA",  "GW"};
        char *storage_type[]= {"UIM",   "NV"};
        printf("###You've got one new %s message, stored to %s index=%d\n", 
                    msg_format[msgRef->e_mode & 1],
                    storage_type[msgRef->e_storage & 1],
                    msgRef->storage_index);  
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_UCS2)
    {
        unsigned char* smsbuf = NULL;

        smsbuf = (char*)malloc(sizeof(char)*QL_SMS_MAX_MT_MSG_LENGTH);
        if(smsbuf == NULL)
        {
            printf("Out of memory");
            return;
        }
        memset(smsbuf, 0, QL_SMS_MAX_MT_MSG_LENGTH);
        UnicodeStrToUTF8Str((unsigned short*)(&msgRef->sms_data[0]),  
                              smsbuf, 
                              QL_SMS_MAX_MT_MSG_LENGTH) ;
        
        printf("\n###You've got one new UCS2 msg from %s at %s, len=%d, content=%s\n", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len,
                    smsbuf); 
        printf("Received UCS raw data:");
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        
        printf("\nAfter convert to UTF8, len=%d, data:", strlen(smsbuf));
        for(i=0; i<strlen(smsbuf); i++)
        {
            printf("%.2X ", smsbuf[i]);
        }
        printf("\n");
        free(smsbuf);
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_BINARY_DATA)
    {
        printf("###You've got one new BINARY msg from %s at %s , len=%d, content=", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len); 
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        printf("\n");
    }
    else //default is GSM-7
    {
        printf("###You've got one new GSM-7 msg from %s at %s content=%s\n", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data);          
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        printf("\n\n");
    }
}

static void ql_wwan_connstate_cb_func(int     profileIdx,
                                      bool    isConnected,
                                      void    *contextPtr)
{
    printf("######### APN%d %s !  ######\n", profileIdx, isConnected ? "Connected" : "DisConnected");
}


void nw_init_cb(void* cb_data)
{
    printf("\nnw_init_cb!\n");    
}

void dsi_net_evt_cb( dsi_hndl_t         hndl,         /**< Handle for which this event is associated. */
                    void              *user_data,    /**< Application-provided user data. */
                    dsi_net_evt_t      evt,          /**< Event identifier. */
                    dsi_evt_payload_t *payload_ptr   /**< Associated event information. */
                  )
{
    int apn_idx = (int)user_data;
    
    printf("\nAPN[%d] dsi_net_evt_cb, evt=%d!\n", apn_idx, evt);  
    switch(evt)
    {
        case DSI_EVT_NET_IS_CONN:
            printf("Call is connected!!\n"); 
            break;
        case DSI_EVT_NET_NO_NET: /**< Call is disconnected. */
            printf("Call is Disconnected!!\n"); 
            g_is_network_ready[apn_idx] = 0;
            break;
        case DSI_EVT_NET_NEWADDR:
            printf("New address is generated!!\n"); 
            break;
        case DSI_EVT_NET_DELADDR:
            printf("Old address deleted !!\n"); 
            break;
        case DSI_EVT_WDS_CONNECTED:
            printf("Network Ready!!\n"); 
            g_is_network_ready[apn_idx] = 1;
            break;
        default:
            break;
    }
}

static void ql_loc_rx_ind_msg_cb(loc_client_handle_type  h_loc,
                                 E_QL_LOC_NFY_MSG_ID_T   e_msg_id,
                                 void                    *pv_data,
                                 void                    *contextPtr)
{
    printf("e_msg_id=%d\n", e_msg_id); 
    switch(e_msg_id)
    {
        case E_QL_LOC_NFY_MSG_ID_STATUS_INFO:
        {
            char *status[]  = {"NONE",  "SESSION_BEGIN",  "SESSION_END",  "ENGINE_ON",  "ENGINE_OFF"};
            E_QL_LOC_STATUS_VALUE_T e_status = *((E_QL_LOC_STATUS_VALUE_T*)pv_data);
            printf("GPS status=%s\n", status[e_status]);  
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_LOCATION_INFO:
        {
            struct tm *utc;
            int64_t timestamp_s;
            QL_LOC_LOCATION_INFO_T *pt_location = (QL_LOC_LOCATION_INFO_T *)pv_data;
            printf("**** flag=0x%X, Latitude = %f, Longitude=%f, accuracy = %f ****\n", 
                        pt_location->flags, 
                        pt_location->latitude, 
                        pt_location->longitude, 
                        pt_location->accuracy);
            timestamp_s = pt_location->timestamp/1000;  // ms --> s
            utc = gmtime((time_t*)&timestamp_s);
            printf("#####Location timestamp info: timestamp=%lld(%02d:%02d:%04d, %02d:%02d)\n", 
                    pt_location->timestamp, 
                    utc->tm_mon + 1, 
                    utc->tm_mday, 
                    utc->tm_year + 1900, 
                    utc->tm_hour,
                    utc->tm_min);             
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_SV_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_NMEA_INFO:
        {
            struct tm *utc;
            int64_t timestamp_s;
            QL_LOC_NMEA_INFO_T  *pt_nmea = (QL_LOC_NMEA_INFO_T  *)pv_data;
            timestamp_s = pt_nmea->timestamp/1000;  // ms --> s
            utc = gmtime((time_t*)&timestamp_s);
            printf("NMEA info: timestamp=%lld(%02d:%02d:%04d, %02d:%02d), length=%d, nmea=%s\n", 
                    pt_nmea->timestamp, utc->tm_mon + 1, utc->tm_mday, utc->tm_year + 1900, utc->tm_hour,
                    utc->tm_min, pt_nmea->length, pt_nmea->nmea); 
            break;
        }
        case E_QL_LOC_NFY_MSG_ID_CAPABILITIES_INFO:
            break;
        case E_QL_LOC_NFY_MSG_ID_AGPS_STATUS:
            break;
        case E_QL_LOC_NFY_MSG_ID_NI_NOTIFICATION:
            break;
        case E_QL_LOC_NFY_MSG_ID_XTRA_REPORT_SERVER:
            break;
    }
}

#if 1 // test TCP long-connection and heartbeat.
static int    g_testclient_sockfd    = 0;
static int g_heartbeat_thread_run = 0;
static int g_client_send_ack_thread_run   = 0;
static pthread_t thrd_keep_alive= 0;
static pthread_t thrd_client_send_ack= 0;
static pthread_t thrd_server_daemon = 0;
static int  g_timer_id = -1; //use fixed timerID = 1, you can change it( 1 < timer_id < 10 ) 


int sms_timer_expire_cb(ql_timer_id timer_id, void *params)
{
    struct timespec time1   = {0, 0};  
    ql_sms_info_t   t_sms_info = {0};

    clock_gettime(CLOCK_REALTIME, &time1);        
    snprintf(t_sms_info.sms_data, sizeof(t_sms_info.sms_data), "HI, Guys! Msg sent at tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 
    memcpy(t_sms_info.src_addr, (char*)params, strlen((char*)params));
    t_sms_info.format = E_QL_SMS_FORMAT_GSM_7BIT;
        
    QL_SMS_Send_Sms(h_sms, &t_sms_info);
}

int heart_beat_timer_expire_cb(ql_timer_id timer_id, void *params)
{
    printf("time_id = %d expired, send heartbeat\n", timer_id);     
    sem_post(&binSem);
    
    return 0;
}


int send_heartbeat_process(void *params)
{
    int     sockfd          = (int)params;
    int     len             = 0;
    char    heartbeat[64]   = {0};
    //char    rcv_buf[128]    = {0};
    struct timespec time1   = {0, 0};  
    
    g_heartbeat_thread_run = 1;
    
    printf("sockfd = %d !!!\n", sockfd);
    
    while(g_heartbeat_thread_run)
    {
        sem_wait(&binSem);

        clock_gettime(CLOCK_REALTIME, &time1);        
        snprintf(heartbeat, sizeof(heartbeat), "heart beat to server, tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 

        len = send(sockfd, heartbeat, strlen(heartbeat), 0);
        if (len < 0)
        {
            printf("Message '%s' failed to send ! "
                    "The error code is %d, error message '%s'\n",
                    heartbeat, errno, strerror(errno));
        }
        else
        {
            printf("News: %s\b >>> Send Done!!!\n", heartbeat);
        }
        // removed as this will cause SIGNAL 13
        //recv(sockfd, rcv_buf, sizeof(rcv_buf), 0);
        //printf("###News from server: %s\n", rcv_buf);
    }
    close(g_testclient_sockfd);
    return 0;
}

static void keep_alive_thread_init(int sockfd)
{
    sem_init(&binSem, 0, 0);
    if (pthread_create(&thrd_keep_alive, NULL, (void*)send_heartbeat_process, (void*)sockfd) != 0)
    {
        printf("Fail to create thread!\n");
        return;
    }
}

int client_send_ack_process(void *params)
{
    int     sockfd          = (int)params;
    int     len             = 0;
    char    ack_msg[64]     = {0};
    char    rcv_buf[128]    = {0};
    struct  timespec time = {0, 0};
    
    g_client_send_ack_thread_run = 1;
    
    printf("sockfd = %d !!!\n", sockfd);
    
    while(g_client_send_ack_thread_run)
    {
        len = recv(sockfd, rcv_buf, sizeof(rcv_buf), 0);
        if(len > 0)
        {
        clock_gettime(CLOCK_REALTIME, &time);
        printf("###News from server: %s\n", rcv_buf);
        printf("####Client Got msg time: tick=%d.%d\n", (int)time.tv_sec, (int)time.tv_nsec);

        snprintf(ack_msg, sizeof(ack_msg), "%s", rcv_buf);

            len = send(sockfd, ack_msg, strlen(ack_msg), 0);
            if (len < 0)
            {
                printf("Message '%s' failed to send ! "
                        "The error code is %d, error message '%s'\n",
                        ack_msg, errno, strerror(errno));
            }
            else
            {
                printf("News: %s\b >>> Send Done!!!\n", ack_msg);
            }
        }
        sleep(1);
    }
    close(g_testclient_sockfd);
    g_testclient_sockfd = 0;
    return 0;
}

static void client_send_ack_thread_init(int sockfd)
{
    if (pthread_create(&thrd_client_send_ack, NULL, (void*)client_send_ack_process, (void*)sockfd) != 0)
    {
        printf("Fail to create thread!\n");
        return;
    }
}

#define MAXBUF 1024

int testapi_client_send_ack(void)
{
    struct sockaddr_in dest;
    char ip_addr[32] = {0};
    int  port = 0;

    if ((g_testclient_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        return -1;
    }    
    
    while(1)
    {
        printf("please input valid ip address: \n");
        scanf("%30s", ip_addr); // won't accept empty string
        
        if (inet_aton(ip_addr, (struct in_addr*)&dest.sin_addr.s_addr) == 0)
        {
            perror(ip_addr);
        }
        else
        {
            break;
        }
    }
        
    printf("please input port number: ");
    scanf("%d", &port);

    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);

    if (connect(g_testclient_sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0)
    {
        perror("Connect");
        return -1;
    }
    printf("Connect OK, sockfd=%d\n", g_testclient_sockfd);
    
    client_send_ack_thread_init(g_testclient_sockfd);
    return 0;
}

int testapi_client(void)
{
    struct sockaddr_in dest;
    char ip_addr[64] = {0};
    int  port = 0;

    bzero(&dest, sizeof(dest));
    if ((g_testclient_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        return -1;
    }    
    
    while(1)
    {
        printf("please input valid ip address: \n");
        scanf("%40s", ip_addr); // won't accept empty string
        
        if (inet_aton(ip_addr, (struct in_addr*)&dest.sin_addr.s_addr) == 0)
        {
            perror(ip_addr);
        }
        else
        {
            break;
        }
    }

    printf("please input port number: ");
    scanf("%d", &port);

    
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    //memset(&(dest.sin_zero), 0, 8);

    if (connect(g_testclient_sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0)
    {
        perror("Connect");
        return -1;
    }
    printf("Connect OK, sockfd=%d\n", g_testclient_sockfd);
    
#if 1 //use qmi_timer to send heartbeat

    /* support user to input heartbeat time interval by gale 2017/08/16 */
    int time_val;
    printf("please input heartbeat time interval(s): ");
    scanf("%d", &time_val);

    if(-1 == start_qmi_timer(heart_beat_timer_expire_cb, time_val, 1, (void *)g_testclient_sockfd))
    {
        return -1;
    }
    keep_alive_thread_init(g_testclient_sockfd);
#else //use system select
    struct timeval tv;
    int retval, maxfd = -1, len;
    char buffer[MAXBUF];
    fd_set rfds;
    char heartbeat[64] = "hello server";
    
    printf("\nReady to start chatting.\n"
            "Direct input messages and \n"
            "enter to send messages to the server\n");

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;

        FD_SET(sockfd, &rfds);
        if (sockfd > maxfd)
        maxfd = sockfd;

        tv.tv_sec = 10;
        tv.tv_usec = 0;

        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            printf("Will exit and the select is error! %s", strerror(errno));
            break;
        }
        else if (retval == 0) //"No message comes, no buttons, continue to wait ...\n"
        {
            struct timespec time1 = {0, 0};   
              
            clock_gettime(CLOCK_REALTIME, &time1);        
            snprintf(heartbeat, sizeof(heartbeat), "heart beat to server, tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 
    
            len = send(sockfd, heartbeat, strlen(heartbeat), 0);
            if (len < 0)
            {
                printf("Message '%s' failed to send ! "
                "The error code is %d, error message '%s'\n",
                heartbeat, errno, strerror(errno));
                break;
            }
            else
            {
                printf("News: %s\b >>> Send Done!!!\n", heartbeat);
            }
            continue;
        }
        else
        {
            if (FD_ISSET(sockfd, &rfds))
            {
                bzero(buffer, MAXBUF+1);
                len = recv(sockfd, buffer, MAXBUF, 0);

                if (len > 0)
                {
                    printf("Successfully received the message: '%s',%d bytes of data\n",
                    buffer, len);
                }
                else
                {
                    if (len < 0)
                    {
                        printf("Failed to receive the message! "
                                "The error code is %d, error message is '%s'\n",
                                errno, strerror(errno));
                    }
                    else
                    {
                        printf("Chat to terminate!\n");
                    }
                    break;
                }
            }
            
            if (FD_ISSET(0, &rfds))
            {
                bzero(buffer, MAXBUF+1);
                fgets(buffer, MAXBUF, stdin);

                if (!strncasecmp(buffer, "quit", 4))
                {
                    printf("Own request to terminate the chat!\n");
                    break;
                }

                len = send(sockfd, buffer, strlen(buffer)-1, 0);
                if (len < 0)
                {
                    printf( "Message '%s' failed to send ! "
                            "The error code is %d, error message '%s'\n",
                            buffer, errno, strerror(errno));
                    break;
                }
                else
                {
                    printf("News: %s\b >>> Send Done! \n", buffer);
                }
            }
        }
    }
    close(sockfd);
#endif
    return 0;
}


void showclient()
{
    int i;
    printf("client amount: %d\n", g_cur_conn_cnt);
    for (i = 0; i < SUPPORTED_CONNECTION_MAX; i++) 
    {
        printf("[%d]:%d  ", i, g_accept_fd[i]);
    }
    printf("\n\n");
}

void server_send_ack(int fd)
{
    char    ack_msg[128]   = {0};
    struct timespec time1   = {0, 0};  
    
    clock_gettime(CLOCK_REALTIME, &time1);  
    
    snprintf(ack_msg, sizeof(ack_msg), "ack from server, tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 

    send(fd, ack_msg, strlen(ack_msg), 0);
}

static void test_server_daemon(int sockfd)
{
    int             i;
    int             ret;
    int             yes = 1;
    char            buf[MAXBUF] = {0};
    socklen_t       sin_size;
    int             sock_fd, new_fd;    // listen on sock_fd, new connection on new_fd
    struct sockaddr_in server_addr;     // server address information
    struct sockaddr_in client_addr;     // connector's address information
    struct timespec time = {0, 0};      // record time get msg  by gale 2017/08/16

    fd_set          fdsr;
    int             maxsock;
    struct timeval  tv;
    
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        return ;
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    {
        perror("setsockopt");
        return ;
    }
        
    server_addr.sin_family = AF_INET;           // host byte order
    server_addr.sin_port = htons(TEST_PORT);    // short, network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;   // automatically fill with my IP
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("bind");
        return ;
    }
    if (listen(sock_fd, SUPPORTED_CONNECTION_MAX) == -1) 
    {
        perror("listen");
        return ;
    }
    printf("\nlisten port %d start!\n", TEST_PORT);
    
    g_cur_conn_cnt = 0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;
    while (g_is_server_daemon_running) 
    {
        // initialize file descriptor set
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);  // add fd
        // timeout setting
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        // add active connection to fd set
        for (i = 0; i < SUPPORTED_CONNECTION_MAX; i++) 
        {
            if (g_accept_fd[i] != 0) 
            {
                FD_SET(g_accept_fd[i], &fdsr);
            }
        }
        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);
        if (ret < 0) 
        {          // error
            perror("select");
            //break;
        } 
        else if (ret == 0) 
        {  // time out
            printf("timeout\n");
            continue;
        }
        // check every fd in the set
        for (i = 0; i < g_cur_conn_cnt; i++) 
        {
            if (FD_ISSET(g_accept_fd[i], &fdsr)) // check which fd is ready
            {
                ret = recv(g_accept_fd[i], buf, sizeof(buf), 0);
                if (ret <= 0) 
                {        // client close
                    printf("ret : %d and client[%d] close\n", ret, i);
                    close(g_accept_fd[i]);
                    FD_CLR(g_accept_fd[i], &fdsr);  // delete fd 
                    g_accept_fd[i] = 0;
                    g_cur_conn_cnt--;
                }
                else 
                {   // receive data
                    if (ret < MAXBUF)
                    {
                        memset(&buf[ret], '\0', 1); // add NULL('\0')
                    }
            //record the time when server get msg by gale 2017/08/16
            clock_gettime(CLOCK_REALTIME, &time);
            printf("####Server Got msg time: tick=%d.%d\n", (int)time.tv_sec, (int)time.tv_nsec);

                    printf("####Server Got: Client[%d] Msg:%s\n", i, buf);
                    server_send_ack(g_accept_fd[i]);
                }
            }
        }
        // check whether a new connection comes
        if (FD_ISSET(sock_fd, &fdsr))  // accept new connection 
        {
            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd <= 0) 
            {
                perror("accept");
                continue;
            }
            // add to fd queue
            if (g_cur_conn_cnt < SUPPORTED_CONNECTION_MAX) 
            {
                char *welcome_msg = "Hello, I'm server!";
                g_accept_fd[g_cur_conn_cnt++] = new_fd;
                printf("new connection client[%d] %s:%d\n", g_cur_conn_cnt,
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                if (new_fd > maxsock)  // update the maxsock fd for select function
                {
                    maxsock = new_fd;
                }
                send(new_fd, welcome_msg, strlen(welcome_msg)+1, 0);
            }
            else 
            {
                printf("max connections arrive, exit\n");
                send(new_fd, "bye", 4, 0);
                close(new_fd);
                //break;   
            }
        }
        showclient();
    }
    // close other connections
    for (i = 0; i < SUPPORTED_CONNECTION_MAX; i++) 
    {
        if (g_accept_fd[i] != 0) 
        {
            close(g_accept_fd[i]);
        }
    }
    return ;
}


static void test_server_daemon_thread_init(int sockfd)
{
    g_is_server_daemon_running = 1;
    if (pthread_create(&thrd_server_daemon, NULL, (void*)test_server_daemon, (void*)sockfd) != 0)
    {
        printf("Fail to create thread!\n");
        return;
    }
}
#endif

#if 1 //UDP test client/server

static int g_is_udp_server_running = 0; 
void *thread_udp_server_recv(void *arg)
{    
    int recvLen = 0; 
    int fd = (int)arg;
    struct sockaddr_in from;
    uint32_t fromlen = sizeof(from);
    unsigned char recvDataBuff[1024] = {0}; 

    printf("udp server thread begin!\r\n");

    while(g_is_udp_server_running)    
    {
        recvLen = recvfrom(fd, recvDataBuff, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if(-1 == recvLen)        
        {
            perror("recvfrom");
            break;
        }

        if(recvLen > 0)
        {
            char    ack_msg[128]   = {0};
            struct timespec time1   = {0, 0};  
            
            printf("###UDP server recv %d byte from %s:%d, Content:%s\n", 
                recvLen, inet_ntoa(from.sin_addr), ntohs(from.sin_port), recvDataBuff);            
            
            clock_gettime(CLOCK_REALTIME, &time1);  
            
            snprintf(ack_msg, sizeof(ack_msg), "this is ack from udp server, tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 

            sendto(fd, ack_msg, sizeof(ack_msg), 0, (struct sockaddr *)&from, sizeof(from)); 
        }
        sleep(1);
    }
    close(fd);

    return NULL;
}

int test_udp_server(void)
{
    int sockfd = 0;    
    struct sockaddr_in local_addr;
    pthread_t tid_udp_recv;

    //create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == sockfd)
    {
        perror("socket");
        return -1;
    }

    //bind server port
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(6789);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&local_addr.sin_zero, 8);
    if(-1 == bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)))
    {
        perror("bind");
    close(sockfd);
        return -1;
    }
    g_is_udp_server_running = 1;
    //start receive thread
    pthread_create(&tid_udp_recv, NULL, thread_udp_server_recv, (void*)sockfd);

    return 0;
}

int test_udp_client(void)  
{  
    int udp_sock_fd         = 0;   
    char buff[128]          = {0};  
    char ip_addr[32]        = {0};
    struct sockaddr_in addr;  
    struct timespec time1   = {0, 0};  
  
    printf("This is a UDP client\n"); 
  
    if ( (udp_sock_fd=socket(AF_INET, SOCK_DGRAM, 0)) <0)  
    {  
        perror("socket");  
        return -1;  
    }  
    
    
    addr.sin_family = AF_INET;  
    addr.sin_port = htons(6789);  
    while(1)
    {
        printf("please input server ip address: \n");
        scanf("%30s", ip_addr); // won't accept empty string
        
        addr.sin_addr.s_addr = inet_addr(ip_addr);  
        if (addr.sin_addr.s_addr == INADDR_NONE)  
        {  
            continue;  
        }
        break;
    }
  
    while (1)  
    {  
        clock_gettime(CLOCK_REALTIME, &time1);  
        
        snprintf(buff, sizeof(buff), "This is udp data from client, tick=%d.%d\n", (int)time1.tv_sec, (int)time1.tv_nsec); 
    
        if( sendto(udp_sock_fd, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {  
            perror("sendto");  
            break;  
        }
        else
        {  
            printf("client send <<<%s>>> success!\n", buff);  
        }
        break;//only send once to avoid block main
    }  
    close(udp_sock_fd);  
      
    return 0;  
}  

#endif


void force_sleep(void)
{
    /* add by gale 2017/08/16 */
    printf("\t######### Module enter sleep #######\n");
    system("echo 0 > /sys/devices/78b1000.uart/clock");
    system("echo -e 'at+qsclk=1\r\n' > /dev/smd8");
}

static void handle_sig(int sig)
{
    printf("######## got signal %d #######\n", sig);
}

void nslookup()
{
    
    char hostname[64];
    char dns_server_ip[20];
    char type;
    
    QUERY_IP_TYPE ip_type;
    hostaddr_info_u resolved_addr;
    int i;
    char ip_str[128];

    //get hostname
    printf("input private domain: ");
    scanf("%s", hostname);
    //Get the DNS servers
    printf("input private network dns ip: ");
    scanf("%s", dns_server_ip);
    //expect ip type
    printf("input ip type(4 or 6): ");
    getchar();
    scanf("%c", &type);

    if (type == '6') {
        ip_type = QUERY_IPV6_E;
    }
    else {
        ip_type = QUERY_IPV4_E;
    }
    
    //resolving...
    QL_nslookup(hostname, dns_server_ip, ip_type, &resolved_addr);
    
    //printf resolved addr
    for (i = 0; i < resolved_addr.addr_cnt; i++) {
         inet_ntop(AF_INET, &resolved_addr.addr[i].s_addr, ip_str, sizeof(ip_str));
         printf("%s has IPv4 address : %s\n\r", hostname, ip_str);
    }
    
    for (i = 0; i < resolved_addr.addr6_cnt; i++) {
         inet_ntop(AF_INET6, &resolved_addr.addr6[i].s6_addr, ip_str, sizeof(ip_str));
         printf("%s has IPv6 address : %s\n\r", hostname, ip_str);
    }

}

/* gpio cb handler */
void gpio_cb_handler(int signal)    
{
    printf("gpio Callback: Have caught signal %d from quectel low consume\n", signal);  
}  

/* register gpio cb handler */
int register_gpio_cb()
{
    int ret, fd, Oflags;
    signal(SIGIO, gpio_cb_handler);
    fd = open("/dev/gpio_dev", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        goto err;
    }
    ret = fcntl(fd, F_SETOWN, getpid());
    if(ret < 0)
    {
        perror("fcntl");
        goto err;
    }

    Oflags = fcntl(fd, F_GETFL);
    if(Oflags < 0)
    {
        printf("fcntl:fasync error!\n");
        goto err;
    }
    ret = fcntl(fd, F_SETFL, Oflags | FASYNC);
    if(ret < 0)
    {
        printf("fcntl:fasync error!\n");
        goto err;
    }
    return 0;
err:
    return -1;
}


int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    value   = 0;
    int    value1  = 0;
    int    ret     = E_QL_OK;
    char   buf[BUF_SIZE] = {0};
    char   buf1[BUF_SIZE]= {0};
    char   phoneNum[32] = {0};
    QL_VCALL_StateHandlerRef_t  h_voicecall = 0;    
    ST_VCALL_CallRef            h_call = 0;
    atc_client_handle_type      h_atc = 0;
    voice_client_handle_type    h_voice     = 0;
    int                         voice_call_id = 0;
    int                         h_loc       = 0;
    
    signal(SIGPIPE, handle_sig);
    
    print_help();

    while(1)
    {
        printf("please input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            int i = 0;
            for(i=0; i<SUPPORTED_APN_MAX; i++)
            {
                if(g_h_handle[i] != NULL)
                {
                    QL_WWAN_StopDataCall(g_h_handle[i]);
                    while(g_is_network_ready[g_cur_apn_idx])
                    {
                        printf("Wait for network disconnect!\n");
                        sleep(1);
                    }
                    QL_WWAN_ReleaseDataSrvHandle(g_h_handle[i]);
                    //QL_WWAN_Deinitialize();
                    g_h_handle[i] = NULL;
                }
            }
            if(g_heartbeat_thread_run)
            {
                g_heartbeat_thread_run = 0;
                if(g_testclient_sockfd != 0)
                {
                    close(g_testclient_sockfd);
                }
                sem_destroy(&binSem);
            }
            if(g_timer_id != -1)
            {
                ret = QL_Timer_Stop(g_timer_id);
                printf("QL_Timer_Stop(%d) ret %d\n", g_timer_id, ret);
            }
            break;
        }
        memset(buf, 0, sizeof(buf));
        switch(cmdIdx)
        {
        case 0://"QL_DEV_GetImei"
            ret = QL_DEV_GetImei(buf, BUF_SIZE);
            printf("ret = %d, IMEI: %s\n", ret, buf);
            break;
        case 1://"QL_DEV_GetFirmwareVersion"
            ret = QL_DEV_GetFirmwareVersion(buf, BUF_SIZE);
            printf("ret = %d, FW Version: %s\n", ret, buf);
            break;
        case 2://"QL_DEV_GetDeviceModel"
            ret = QL_DEV_GetDeviceModel(buf, BUF_SIZE);
            printf("ret = %d, Dev Model: %s\n", ret, buf);
            break;
        case 3://"QL_DEV_GetMeid"
            ret = QL_DEV_GetMeid(buf, BUF_SIZE);
            printf("ret = %d, MEID: %s\n", ret, buf);
            break;
        case 4://"QL_DEV_GetEsn"
            ret = QL_DEV_GetEsn(buf, BUF_SIZE);
            printf("ret = %d, ESN: %s\n", ret, buf);
            break;
            
        case 40://"QL_LOC_ Init"
        {
            int                     test_cnt    = 0;
            int                     bitmask     = 0;
            QL_LOC_POS_MODE_INFO_T  t_mode      = {0};
            QL_LOC_LOCATION_INFO_T  t_loc_info  = {0};

            ret = QL_LOC_Client_Init(&h_loc);
            printf("QL_LOC_Client_Init ret %d with h_loc=%d\n", ret, h_loc);
            if(ret != 0)
            {
                break;
            }
            
            ret = QL_LOC_AddRxIndMsgHandler(ql_loc_rx_ind_msg_cb, (void*)h_loc);
            if(ret != 0)
            {
                break;
            }                
            
            printf("Please input indication bitmask(NiNfy|AGPS|XTRA|UTC|CAP|NMEA|SV|Status|Location)(HEX Base, i.e. 1FF):\n", ret);
            scanf("%x", &bitmask);
            
            /* Set what we want callbacks for */
            ret = QL_LOC_Set_Indications(h_loc, bitmask);
            printf("#### QL_LOC_Set_Indications bitmask=0x%X ret %d\n", bitmask, ret);
            if(ret != 0)
            {
                printf("QL_LOC_Set_Indications Fail, ret %d\n", ret);
                break;
            }

            //You need set it as you wish
            t_mode.mode                 = E_QL_LOC_POS_MODE_STANDALONE;
            t_mode.recurrence           = E_QL_LOC_POS_RECURRENCE_PERIODIC;
            t_mode.min_interval         = 1000;  //report nmea frequency 1Hz
            t_mode.preferred_accuracy   = 50;    // <50m
            t_mode.preferred_time       = 90;    // 90s
            ret = QL_LOC_Set_Position_Mode(h_loc, &t_mode);
            printf("#### QL_LOC_Set_Position_Mode ret %d\n", ret);
            if(ret != 0)
            {
                printf("QL_LOC_Set_Indications Fail, ret %d\n", ret);
                break;
            }
            break;
        }
        
        case 41://"QL_LOC_Start_Navigation"
        {
            ret = QL_LOC_Start_Navigation(h_loc);
            printf("QL_LOC_Start_Navigation ret=%d\n", ret);
            break;
        }
        
        case 42://"QL_LOC_Stop_Navigation"
        {
            ret = QL_LOC_Stop_Navigation(h_loc);
            printf("QL_LOC_Stop_Navigation ret=%d\n", ret);
            break;
        }       
        
        case 43://"QL_LOC_Get_Current_Location"
        {
            int                     timeout_sec = 60;
            QL_LOC_LOCATION_INFO_T  t_loc_info  = {0};

            ret = QL_LOC_Get_Current_Location(h_loc, &t_loc_info, timeout_sec);
            printf(" QL_LOC_Get_Current_Location ret %d\n", ret);
            if(ret < 0)
            {
                if(ret == -2)
                {// -2: timeout, may need try again
                    printf("QL_LOC_Get_Current_Location timeout, try again!\n");
                }
                else
                {
                    printf("QL_LOC_Get_Current_Location Fail, ret %d\n", ret);
                    break;
                }
            }
            else
            {
                printf("**** Latitude = %lf, Longitude=%lf, altitude=%lf, accuracy = %f ****\n", 
                        t_loc_info.latitude, t_loc_info.longitude, t_loc_info.altitude, t_loc_info.accuracy);
            }
            break;
        }
        
        case 44://"QL_LOC_Client_Deinit"
        {
            ret = QL_LOC_Client_Deinit(h_loc);
            printf("QL_LOC_Client_Deinit ret=%d\n", ret);
            break;
        }

        case 45://"QL_LOC_Xtra_InjectFileData"
        {
            char    file_path_name[128] = {0};

            printf("please input xtra data file path+name: \n");
            scanf("%s", file_path_name);
            
            ret = QL_LOC_Xtra_InjectFile(h_loc, file_path_name);
            printf("QL_LOC_Xtra_InjectFile ret=%d\n", ret);
            break;
        }
        
        case 50://"QL_ATC_Client_Init"
        {
            ret = QL_ATC_Client_Init(&h_atc);
            printf("QL_ATC_Client_Init ret=%d with h_atc=0x%x\n", ret, h_atc);
            break;
        }
        
        case 51://"QL_ATC_Send_Cmd"
        {
            memset(atc_cmd_req,  0, sizeof(atc_cmd_req));
            memset(atc_cmd_resp, 0, sizeof(atc_cmd_resp));
            
            printf("please input atc cmd: ");
            scanf("%s", atc_cmd_req);
            ret = QL_ATC_Send_Cmd(h_atc, atc_cmd_req, atc_cmd_resp, ATC_RESP_CMD_MAX_LEN);
            printf("QL_ATC_Send_Cmd \"%s\" ret=%d with resp=\n%s\n", atc_cmd_req, ret, atc_cmd_resp);
            break;
        }
        
        case 52://"QL_ATC_Client_Deinit"
        {
            ret = QL_ATC_Client_Deinit(h_atc);
            printf("QL_ATC_Client_Deinit ret=%d\n", ret);
            break;
        }

        case 60://"QL_SMS_Client_Init"
        {
            ret = QL_SMS_Client_Init(&h_sms);
            printf("QL_SMS_Client_Init ret=%d with h_sms=0x%x\n", ret, h_sms);
            break;
        }
        case 61://"QL_SMS_Send_Sms"
        {
            int                 i       = 0;
            int                 len     = 0;
            E_QL_SMS_FORMAT_T   e_format= 0;
            char                sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
            ql_sms_info_t       *pt_sms_info = NULL;
            
            pt_sms_info = (ql_sms_info_t*)malloc(sizeof(ql_sms_info_t));
            if(pt_sms_info == NULL)
            {
                printf("Malloc fail!\n");
                break;
            }
            memset(pt_sms_info, 0, sizeof(ql_sms_info_t));

            printf("please input dest phone number: \n");
            scanf("%s", pt_sms_info->src_addr);

            printf("please input sms encoding type(0:GSM-7, 1:Binary, 2:UCS2): \n");
            scanf("%d", &e_format);
            e_format = e_format & 0x03;

            if((e_format == E_QL_SMS_FORMAT_GSM_7BIT) || (e_format == E_QL_SMS_FORMAT_UCS2))
            {
                printf("please input message content: \n");
                getchar();//Add this on purpose or the following fgets will be skipped.
                fgets(sms_buf, QL_SMS_MAX_MT_MSG_LENGTH, stdin);
                len= strlen(sms_buf); //to skip 0x0A
                sms_buf[len-1] = '\0';
                printf("textData[%d]:%s\n", len, sms_buf);
                            
                printf("input %d byte data:", len);
                for(i=0; i<len; i++)
                {
                    printf("%.2X ", sms_buf[i]);
                }
                printf("\n");
            }
            else
            {
                printf("please input binary data counts in bytes: \n");
                do
                {
                    i = scanf("%d", &len);
                }while(i != 1);
                
                printf("please input binary data: \n");
                for(i=0; i<len; i++)
                {
                    printf("Byte[%d]=", i);
                    scanf("%2X", &sms_buf[i]);
                }
            }
            
            if(e_format == E_QL_SMS_FORMAT_GSM_7BIT)
            {
                memcpy(pt_sms_info->sms_data, sms_buf, QL_SMS_MAX_MT_MSG_LENGTH);
                pt_sms_info->sms_data_len = strlen(sms_buf);
            }
            else if(e_format == E_QL_SMS_FORMAT_BINARY_DATA)
            {
                memcpy(pt_sms_info->sms_data, sms_buf, len);
                pt_sms_info->sms_data_len = len;
            }
            else
            {
                len = UTF8StrToUnicodeStr(sms_buf, 
                                          (uint16_t*)pt_sms_info->sms_data, 
                                          len);// Got BE data
                pt_sms_info->sms_data_len = len * 2;//in bytes
                
                printf("raw UTF-16 len=%d, data:", pt_sms_info->sms_data_len);
                for(i=0; i<pt_sms_info->sms_data_len; i++)
                {
                    printf("%.2X ", pt_sms_info->sms_data[i]);
                }
                printf("\n");
            }

            pt_sms_info->format = e_format;
                       
            ret = QL_SMS_Send_Sms(h_sms, pt_sms_info);
            printf("#QL_SMS_Send_Sms ret=%d \n", ret);
            free(pt_sms_info);
            break;
        }
        
        case 62://"QL_ATC_Client_Deinit"
        {
            ret = QL_SMS_Client_Deinit(h_sms);
            printf("QL_SMS_Client_Deinit ret=%d \n", ret);
            break;
        }
        
        case 63://"QL_SMS_AddRxMsgHandler"
        {
            ret = QL_SMS_AddRxMsgHandler(ql_sms_cb_func, (void*)h_sms);
            printf("QL_SMS_AddRxMsgHandler ret=%d \n", ret);
            break;
        }

        case 64://"QL_SMS_SetRouteList"
        {
            int i;
            ql_sms_route_info_list_t t_route_list = {0};
            
            for (i = 0; i < E_QL_SMS_MESSAGE_CLASS_MAX; i++)
            {
                t_route_list.route_list[i].message_class    = i;
                t_route_list.route_list[i].route_storage    = E_QL_SMS_STORAGE_TYPE_UIM;
                t_route_list.route_list[i].receipt_action   = E_QL_SMS_STORE_AND_NOTIFY;//WMS_TRANSFER_AND_ACK_V01;//
            }
            t_route_list.route_list[E_QL_SMS_MESSAGE_CLASS_0].receipt_action  = WMS_TRANSFER_ONLY_V01;        
            t_route_list.route_list[E_QL_SMS_MESSAGE_CLASS_2].route_storage   = E_QL_SMS_STORAGE_TYPE_UIM; 
            t_route_list.route_list[E_QL_SMS_MESSAGE_CLASS_2].receipt_action  = WMS_TRANSFER_ONLY_V01;        
            
            ret = QL_SMS_SetRouteList(h_sms, &t_route_list);
            printf("QL_SMS_SetRouteList ret=%d \n", ret);
            break;
        }
        case 65://"QL_SMS_DeleteFromStorage"
        {
            ql_sms_storage_info_t    t_sms_storage_info = {0};

            printf("please input storage type(0:UIM, 1:NV): \n");
            scanf("%d", &t_sms_storage_info.e_storage);

            printf("please input sms mode(0:CDMA, 1:GW): \n");
            scanf("%d", &t_sms_storage_info.e_mode);
            
            printf("please input storage index: \n");
            scanf("%d", &t_sms_storage_info.storage_idx);   
            
            t_sms_storage_info.e_storage    &= 1;
            t_sms_storage_info.e_mode       &= 1;
            
            ret = QL_SMS_DeleteFromStorage(h_sms, &t_sms_storage_info);
            printf("QL_SMS_DeleteFromStorage ret=%d \n", ret);
            break;
        }

        case 80://"set_selected_apn_idx"
        {
            do
            {
                printf("please input valid apn index(1~5): ");
                scanf("%d", &g_cur_apn_idx);
                if(g_cur_apn_idx > SUPPORTED_APN_MAX)
                {
                    continue;
                }
                g_cur_apn_idx -= 1;//used as array index
            }while(0);
            break;
        }            
        case 90://"test_sleep_wakeup"
            break;
            
        case 97://"testapi_client(work as client, send msg to server)"
            testapi_client();
            break;
            
        case 98://"stop test client"
        {
            sem_destroy(&binSem);
            g_heartbeat_thread_run = 0;
            break;
        }
            
        case 99://"testapi_server(work as server, receive msg and ack msg+ack)"
        {
            test_server_daemon_thread_init((int)g_h_handle[g_cur_apn_idx]);
            break;
        }
        case 100://"stop test server"
        {
            g_is_server_daemon_running = 0;
            break;
        }
        
        case 101://"test_udp_server"
        {
            test_udp_server();
            break;
        }
        case 102://"stop test_udp_server"
        {
            g_is_udp_server_running = 0;
            break;
        }

        
        case 103://"test_udp_server"
        {
            test_udp_client();
            break;
        }
    
        
        case 104://"testapi_client_send_ack"
        {
            testapi_client_send_ack();
            break;
        }

        case 110://"start_qmi_timer to send heartbeat"
        {
            int timeout = 0;
            int repeat_flag = 0;  
            printf("please input timeout(s) and repeat flag, i.e 10 0/1: ");
            scanf("%d%d", &timeout, &repeat_flag);
            start_qmi_timer(heart_beat_timer_expire_cb, timeout, repeat_flag, (void *)g_testclient_sockfd);
            break;
        }

        case 111://"auto sleep onoff"
        {
            //force_sleep();
            int onoff = 0;  
            printf("please input autosleep onoff value(on:1, off:0): ");
            scanf("%d", &onoff);
            Ql_Autosleep_Enable(onoff!=0);
            break;
        }
        
        case 112://"start_qmi_timer to send sms"
        {
            int timeout = 0;
            int repeat_flag = 0;  
            
            printf("please input dest phone number: ");
            scanf("%s", phoneNum);
            
            printf("please input timeout(s) and repeat flag, i.e 10 0/1: ");
            scanf("%d%d", &timeout, &repeat_flag);
            g_timer_id = start_qmi_timer(sms_timer_expire_cb, timeout, repeat_flag, (void*)phoneNum);
            break;
        }
        
        case 120:    /*resolve private network domain*/
        {
            nslookup();
            break;
        }

        case 130:   /*forbid indication*/
        {
            static uint32_t forbid_mask = 0;
            printf("input mask value with hex(bit6:dsd, bit5:voice, bit4:wms, bit3:uim, bit2:pbm, bit1:wds, bit0:nas): \n");
            scanf("%x", &forbid_mask);
            printf("mask value: 0x%x, %d\n", forbid_mask, forbid_mask);
            ret = QL_NW_ForbidInd(forbid_mask);
            printf("QL_NW_ForbidInd ret = %d\n", ret);
            break;
        }

        case 132:    /* register gpio cb handler */
        {
            ret = register_gpio_cb();
            if(ret == 0)
                printf("register gpio callback successfully\n");
            else
                printf("register gpio callback failed\n");

            break;
        }

        case 150://"QL_Voice_Call_Client_Init"
        {
            ret = QL_Voice_Call_Client_Init(&h_voice);
            printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);
            break;
        }
        
        case 151://"QL_Voice_Call_Client_Deinit"
        {
            ret = QL_Voice_Call_Client_Deinit(h_voice);
            printf("QL_Voice_Call_Client_Deinit ret = %d\n", ret);
            break;
        }
        case 152://"QL_Voice_Call_AddStateHandler"
        {
            ret = QL_Voice_Call_AddStateHandler(h_voice, ql_voice_call_cb_func, &voice_call_id);
            printf("QL_Voice_Call_AddStateHandler ret = %d\n", ret);
            break;
        }
        case 153://"QL_Voice_Call_RemoveStateHandler"
        {
            ret = QL_Voice_Call_RemoveStateHandler(h_voice);
            printf("QL_Voice_Call_RemoveStateHandler ret = %d\n", ret);
            break;
        }
        case 154://"QL_Voice_Call_Start"
        {
            char    PhoneNum[32]                  = {0}; 

            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);
            
            ret = QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum, &voice_call_id);
            printf("QL_Voice_Call_Start ret = %d, with voice_call_id=%d\n", ret, voice_call_id);
            break;
        }
        case 155://"QL_Voice_Call_End"
        {
            ret = QL_Voice_Call_End(h_voice, voice_call_id);
            printf("QL_Voice_Call_End voice_call_id=%d, ret = %d\n", voice_call_id, ret);
            break;
        }
        case 156://"QL_Voice_Call_Answer"
        {
            ret = QL_Voice_Call_Answer(h_voice, voice_call_id);
            printf("QL_Voice_Call_Answer ret = %d\n", ret);
            break;
        }
        case 157://"QL_Voice_Call_Hold"
        {
            ret = QL_Voice_Call_Hold(h_voice);
            printf("QL_Voice_Call_Hold ret = %d\n", ret);
            break;
        }
        case 158://"Ql_Set_FirmwareRestore"
        {
            ret = Ql_Set_FirmwareRestore();
            printf(" ret = %d\n", ret);
            break;
        }
        case 159://"Ql_Get_RestoreCnt"
        {
			u32 System_Cnt=0,modem_Cnt =0;
            ret = Ql_Get_RestoreCnt(&System_Cnt, &modem_Cnt);
            printf("linux system restored Cnt= %d, modem restored Cnt=%d\n", System_Cnt,modem_Cnt);
            break;
        }

        default:
            print_help();
            break;   
        }
    }
    return 0;
}
