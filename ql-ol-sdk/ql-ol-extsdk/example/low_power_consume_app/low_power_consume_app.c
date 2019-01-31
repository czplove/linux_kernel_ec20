#include <ql_oe.h>
#include "ql_qcmap_client_api.h"

#define GPIO_NOTIFY "/dev/gpio_notify"
#define IRQ_EDGE "/sys/devices/soc:quec,quectel-low-consume/irq_edge"

//#define SMS
//#define VOICECALL
//#define DATA

static int h_sms, h_voicecall, h_call;
static volatile boolean deamon_flag = TRUE;
static boolean autoconnect = TRUE;
static int wakelock_fd;
static int wakelock_flag = 0;
static int notify_fd;
static int sockfd = 0;
static pthread_t pid = 0;

/*********************** SIGTERM signal handler *************************/
static void ql_sighandler(int signal)
{
    switch(signal)
    {
    case SIGTERM:
    case SIGINT:
        QL_SMS_Client_Deinit(h_sms);
        QL_VCALL_RemoveStateHandler(h_voicecall);
        QL_VCALL_End(h_call);
        Ql_NW_MobileAP_Disable();
        if(sockfd > 0)
            close(sockfd);
        if(pid > 0)
            pthread_join(pid, NULL);
        if(wakelock_flag == 1)
        {
            Ql_SLP_WakeLock_Unlock(wakelock_fd);
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
        }
        exit(1);
        break;

    default:
        QL_USER_LOG("Receiver unexpected signal %s\n",signal);
        break;
    }
}


/**************************  single apn ******************************/
static void ql_qcmapclient_nw_status_cb(ql_wwan_nw_staus_e_type status)
{
    boolean connected;

    switch (status) {
    case QL_WWAN_NW_CONNECTING:
        QL_USER_LOG("APPS connecting wwan...\n\r");
        break;

    case QL_WWAN_NW_CONNECTED:
        QL_USER_LOG("APPS connect wwan done\n\r");
        connected = TRUE;
        break;
    
    case QL_WWAN_NW_CONNECTING_TIMEOUT:
        QL_USER_LOG("APPS connect wwan timeout\n\r");
    case QL_WWAN_NW_DISCONNECTED:
        QL_USER_LOG("APPS disconnect wwan\n\r");
        connected = FALSE;
        if (!autoconnect) {
            deamon_flag = FALSE;
        }
        break;


    default:
        QL_USER_LOG("APPS connect wwan unkonwn evt\n\r");
        break;
        
    }

    return;
}

void single_apn()
{
    Ql_NW_MobileAP_Init();    
    Ql_NW_MoblieAP_AutoConnect_Set(TRUE);
    Ql_NW_MoblieAP_AutoConnect_Get(&autoconnect);
    
    Ql_NW_MobileAP_Enable(10, ql_qcmapclient_nw_status_cb);
}



/**************************  SMS ******************************/
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

unsigned char* UnicodeStrToUTF8Str (unsigned short *unicode_str,  
                                    unsigned char   *utf8_str, 
                                    int             utf8_str_size)  
{  
    int unicode = 0;  
    unsigned char *e = NULL, *s = NULL;  
    unsigned char utf8_ch[8];   
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

static void ql_sms_cb_func(QL_SMS_MsgRef msgRef, void* contextPtr)
{
    int i;
    if(wakelock_flag != 1)
    {
        if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
        {
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
            QL_USER_LOG("Lock wakelock failed! \n");
            return;
        }
        ioctl(notify_fd, 1);	//pull up notify_pin notify mcu
        wakelock_flag = 1;
    }

    if(msgRef->format == E_QL_SMS_FORMAT_UCS2)
    {
        unsigned char smsbuf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
        UnicodeStrToUTF8Str((unsigned short*)(&msgRef->sms_data[0]),  
                              smsbuf, 
                              QL_SMS_MAX_MT_MSG_LENGTH) ;
        
        QL_USER_LOG("\n###You've got one new UCS2 msg from %s at %s, len=%d, content=%s\n", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len,
                    smsbuf); 
        QL_USER_LOG("Received UCS raw data:");
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        
        QL_USER_LOG("\nAfter convert to UTF8, len=%d, data:", strlen(smsbuf));
        for(i=0; i<strlen(smsbuf); i++)
        {
            printf("%.2X ", smsbuf[i]);
        }
        QL_USER_LOG("\n");
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_BINARY_DATA)
    {
        QL_USER_LOG("###You've got one new BINARY msg from %s at %s , len=%d, content=", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data_len); 
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        QL_USER_LOG("\n");
    }
    else //default is GSM-7
    {
        QL_USER_LOG("###You've got one new GSM-7 msg from %s at %s content=%s\n", 
                    msgRef->src_addr,
                    msgRef->timestamp,
                    msgRef->sms_data);          
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        QL_USER_LOG("\n\n");
    }
}

void sms()
{
    int ret;
    ret = QL_SMS_Client_Init(&h_sms);
    QL_USER_LOG("QL_SMS_Client_Init ret=%d with h_sms=0x%x\n", ret, h_sms);
    ret = QL_SMS_AddRxMsgHandler(ql_sms_cb_func, (void*)h_sms);
    QL_USER_LOG("QL_SMS_AddRxMsgHandler ret=%d \n", ret);
}



/**************************  Voice Call ******************************/
static void ql_vcall_cb_func(ST_VCALL_CallRef reference, const char* identifier,
                             E_QL_VCALL_STATE_T  event,  void* contextPtr)
{
    ST_VCALL_CallRef *ph_call = (ST_VCALL_CallRef *)contextPtr;

    if(wakelock_flag != 1)
    {
        if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
        {
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
            QL_USER_LOG("Lock wakelock failed! \n");
            return;
        }
        ioctl(notify_fd, 1);	//pull up notify mcu
        wakelock_flag = 1;
    }
    
    if(*ph_call == 0)
    {
        *ph_call = reference;
    }
    QL_USER_LOG("######### Call id=%d, PhoneNum:%s, event=%d!  ######\n", reference, identifier, event);
}

void voicecall()
{
    h_voicecall = QL_VCALL_AddStateHandler(ql_vcall_cb_func, (void*)&h_call);
    QL_USER_LOG("QL_VCALL_AddStateHandler h_voicecall = %d\n", h_voicecall);
}


/************************** gpio irq *****************************/
/* gpio cb handler */
void gpio_cb_handler(int signal)
{
	char edge[8] = {0};
	int edge_fd;

	QL_USER_LOG("gpio irq: Have caught signal %d from driver\n", signal);

	if(wakelock_flag != 1)
	{
            if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
       	    {
            	Ql_SLP_WakeLock_Destroy(wakelock_fd);
                QL_USER_LOG("Lock wakelock failed! \n");
                return;
            }
            wakelock_flag = 1;
	}

	edge_fd = open(IRQ_EDGE, O_RDONLY | O_NONBLOCK);
	if(edge_fd < 0)
	{
		perror("open");
		return;
	}
	

	if(read(edge_fd, edge, sizeof(edge)) < 0)
	{
		perror("read");
		return;
	}
	QL_USER_LOG("edge: %s\n", edge);

	if(strncmp(edge, "falling", 7) == 0)
	{
		QL_USER_LOG("do something before enter sleep mode\n");
		QL_USER_LOG("module will enter sleep mode\n");
		if(wakelock_flag == 1)
    		{
    			wakelock_flag = 0;
			ioctl(notify_fd, 0);	//pull down notify mcu
			QL_USER_LOG("pull down notify mcu\n");
	   		if (Ql_SLP_WakeLock_Unlock(wakelock_fd) != 0)
	   		{
	   			Ql_SLP_WakeLock_Destroy(wakelock_fd);
	   			close(edge_fd);
	   			QL_USER_LOG("Unlock wakelock failed!\n");
	   			return;
	   		}
		}
		
	}
	else if(strncmp(edge, "rising", 6) == 0)
	{
		if(wakelock_flag != 1)
	    	{
			if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
			{
				Ql_SLP_WakeLock_Destroy(wakelock_fd);
				close(edge_fd);
				QL_USER_LOG("Lock wakelock failed!\n");
				return;
			}
			wakelock_flag = 1;
		}
		ioctl(notify_fd, 1);	//pull up notify mcu
		QL_USER_LOG("pull up notify mcu\n");
	}
	close(edge_fd);
}  


/* register gpio cb handler */
int register_gpio_cb()
{
	int ret, Oflags;

	signal(SIGIO, gpio_cb_handler);
	notify_fd = open(GPIO_NOTIFY, O_RDWR);
	if (notify_fd < 0)
	{
		perror("open");
		goto err;
	}
	ret = fcntl(notify_fd, F_SETOWN, getpid());
	if(ret < 0)
	{
		perror("fcntl");
		goto err;
	}

	Oflags = fcntl(notify_fd, F_GETFL);
	if(Oflags < 0)
	{
		perror("fcntl");
		goto err;
	}
	ret = fcntl(notify_fd, F_SETFL, Oflags | FASYNC);
	if(ret < 0)
	{
		perror("fcntl");
		goto err;
	}
        QL_USER_LOG("register gpio irq callback ok\n");
	return 0;
err:
	return -1;
}


/****** after dial apn, we connect to server, and then module enter sleep mode,
 ****** waiting server send msg to wakeup module ******/
void client_rev_process(void *params)
{
    int     sockfd          = (int)params;
    int     len             = 0;
    char    ack_msg[64]     = {0};
    char    rcv_buf[128]    = {0};
    struct  timespec time = {0, 0};
   
    while(1)
    {
        len = recv(sockfd, rcv_buf, sizeof(rcv_buf), 0);
        if(wakelock_flag != 1)
        {
            if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
            {
                Ql_SLP_WakeLock_Destroy(wakelock_fd);
                QL_USER_LOG("Lock wakelock failed! \n");
                return;
            }
            ioctl(notify_fd, 1);	//pull up notify_pin notify mcu
            wakelock_flag = 1;
        }

        if(len > 0)
        {
            clock_gettime(CLOCK_REALTIME, &time);
            QL_USER_LOG("###Msg from server: %s\n", rcv_buf);
            QL_USER_LOG("####Client Got msg time: tick=%d.%d\n", (int)time.tv_sec, (int)time.tv_nsec);

            snprintf(ack_msg, sizeof(ack_msg), "%s", rcv_buf);

            len = send(sockfd, ack_msg, strlen(ack_msg), 0);
            if (len < 0)
            {
                QL_USER_LOG("Message '%s' failed to send ! "
                        "The error code is %d, error message '%s'\n",
                        ack_msg, errno, strerror(errno));
            }
            else
            {
                QL_USER_LOG("News: module >>> Ack to server!!!\n");
            }
        }

    }
}

int tcp_client(int argc, char **argv)
{
    if(argc != 3 )
    {
        QL_USER_LOG("#####usage: ./low_power_consume_app server_ip port #####\n");
        goto tcp_err;
    }

    struct sockaddr_in dest;
    char ip_addr[32] = {0};
    int  port = atoi(argv[2]);

    /* start dial single apn */
    single_apn();
    sleep(3);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        QL_USER_LOG("Socket error\n");
        goto tcp_err;
    }    
    
    strcpy(ip_addr, argv[1]);
 
    if (inet_aton(ip_addr, (struct in_addr*)&dest.sin_addr.s_addr) == 0)
    {
        QL_USER_LOG("ip %s is invalid\n", ip_addr);
        goto tcp_err;
    }

    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0)
    {
        QL_USER_LOG("Connect error\n");
        goto tcp_err;
    }
    QL_USER_LOG("######## Connect server %s %d OK #########\n", ip_addr, port);
    
    if (pthread_create(&pid, NULL, (void*)client_rev_process, (void*)sockfd) != 0)
    {
        QL_USER_LOG("Fail to create thread!\n");
        goto tcp_err;
    }
    return 0;

tcp_err:
    return -1;
}


/*************************** Main ***********************************/
int main(int argc, char **argv)
{
    signal(SIGTERM, ql_sighandler);
    signal(SIGINT, ql_sighandler);

    if(daemon(1, 1) < 0)   
    {  
        perror("daemon");  
        exit(1);  
    }

    /* user also could integrate sms, voicecall, data */
#ifdef SMS
    sms();
#endif
#ifdef VOICECALL
    voicecall();
#endif
#ifdef DATA
    if(tcp_client(argc, argv) < 0)
        kill(getpid(), SIGTERM); 
#endif
    
    if(register_gpio_cb() < 0)
        kill(getpid(), SIGTERM); 

    wakelock_fd = Ql_SLP_WakeLock_Create("low_power_consume", sizeof("low_power_consume"));
    if(wakelock_flag != 1)
    {
        if (Ql_SLP_WakeLock_Lock(wakelock_fd) != 0)
        {
            Ql_SLP_WakeLock_Destroy(wakelock_fd);
            QL_USER_LOG("Lock wakelock failed! \n");
            return;
        }
        wakelock_flag = 1;
    }

    Ql_Autosleep_Enable(1);

    while (deamon_flag) {
        sleep(1);
    }

    return 0;
}
