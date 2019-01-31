#include <ql_oe.h>
#include "test_base.h"

static sms_client_handle_type      h_sms;
extern func_api_test_t t_sms_test;
st_api_test_case at_sms_testlist[] = 
{
    {0,    "QL_SMS_Client_Init"},
    {1,    "QL_SMS_Send_Sms"},
    {2,    "QL_SMS_Client_Deinit"},
    {3,    "QL_SMS_AddRxMsgHandler"},
    {4,    "QL_SMS_SetRouteList (Parse sms without store by default, if store, it won't parse)"},
    {5,    "QL_SMS_DeleteFromStorage"},
    {6,    "QL_SMS_SetPreferredStorage"},

    {-1,   "Return to main menu"}
};

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
		if (msgRef->user_data_head_valid)
		{
			printf("\n###You've got one new UCS2 msg from %s at %s, total_segments:%d, seg_number:%d, reference_number:%02x, len=%d, content=%s\n", 
							msgRef->src_addr,
							msgRef->timestamp,
							msgRef->user_data_head.total_segments,
							msgRef->user_data_head.seg_number,
							msgRef->user_data_head.reference_number,
							msgRef->sms_data_len,
							smsbuf); 
		}
		else
		{
	        printf("\n###You've got one new UCS2 msg from %s at %s, len=%d, content=%s\n", 
		                    msgRef->src_addr,
		                    msgRef->timestamp,
		                    msgRef->sms_data_len,
		                    smsbuf); 
    	}
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
    	if (msgRef->user_data_head_valid)
    	{
        	printf("###You've got one new BINARY msg from %s at %s , total_segments:%d, seg_number:%d, reference_number:%02x, len=%d, content=", 
							msgRef->src_addr,
							msgRef->timestamp,
							msgRef->user_data_head.total_segments,
							msgRef->user_data_head.seg_number,
							msgRef->user_data_head.reference_number,
							msgRef->sms_data_len); 
    	}
		else
		{
        	printf("###You've got one new BINARY msg from %s at %s , len=%d, content=", 
							msgRef->src_addr,
							msgRef->timestamp,
							msgRef->sms_data_len); 
		}
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        printf("\n");
    }
    else //default is GSM-7
    {
    	if (msgRef->user_data_head_valid)
    	{
        	printf("###You've got one new GSM-7 msg from %s at %s, total_segments:%d, seg_number:%d, reference_number:%02x, content=%s\n", 
		                    msgRef->src_addr,
		                    msgRef->timestamp,
							msgRef->user_data_head.total_segments,
		                    msgRef->user_data_head.seg_number,
					    	msgRef->user_data_head.reference_number,
		                    msgRef->sms_data);          
    	}
		else
		{
			printf("###You've got one new GSM-7 msg from %s at %s, content=%s\n", 
		                    msgRef->src_addr,
		                    msgRef->timestamp,
		                    msgRef->sms_data);   
		}
        for(i=0; i<msgRef->sms_data_len; i++)
        {
            printf("%.2X ", msgRef->sms_data[i]);
        }
        printf("\n\n");
    }
}

#define BUF_SIZE 32
static int test_sms(void)
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char    buf[BUF_SIZE] = {0};    
        
    show_group_help(&t_sms_test);

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
        case 0://"QL_SMS_Client_Init"
        {
            ret = QL_SMS_Client_Init(&h_sms);
            printf("QL_SMS_Client_Init ret=%d with h_sms=0x%x\n", ret, h_sms);
            break;
        }
        case 1://"QL_SMS_Send_Sms"
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
        
        case 2://"QL_ATC_Client_Deinit"
        {
            ret = QL_SMS_Client_Deinit(h_sms);
            printf("QL_SMS_Client_Deinit ret=%d \n", ret);
            break;
        }
        
        case 3://"QL_SMS_AddRxMsgHandler"
        {
            ret = QL_SMS_AddRxMsgHandler(ql_sms_cb_func, (void*)h_sms);
            printf("QL_SMS_AddRxMsgHandler ret=%d \n", ret);
            break;
        }

        case 4://"QL_SMS_SetRouteList"
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
        case 5://"QL_SMS_DeleteFromStorage"
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
        
        case 6://"QL_SMS_SetPreferredStorage"
        {
            E_QL_SMS_STORAGE_TYPE_T e_storage_type;
            
            printf("please input storage type(0:UIM, 1:NV): \n");
            scanf("%d", &e_storage_type);
            
            ret = QL_SMS_SetPreferredStorage(h_sms, e_storage_type);
            printf("QL_SMS_SetPreferredStorage ret=%d \n", ret);
            break;
        }
    
        default:
            show_group_help(&t_sms_test);
        }
    }
    return 0;
}


func_api_test_t t_sms_test = {"mcm_sms", at_sms_testlist, test_sms};


