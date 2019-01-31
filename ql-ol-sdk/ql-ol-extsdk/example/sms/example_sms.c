/*********************************************************************************************
 *
 *                  QUECTEL - Build a Smarter World.
 *
 * Copyright Statement:
 * --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Quectel Co., Ltd. 2018
 *
 * Filename:
 * ---------
 *   example_sms.c
 *
 * Project:
 * --------
 *   MDM9x07 - OpenLinux
 *
 * Description:
 * ------------
 *   This example demonstrates how to use SMS function with APIs in OpenLinux.
 *
 *============================================================================
 * HISTORY 
 * ---------
 *   2017/12/27   Vicent GAO    Create this file by QCM9XOL00002C001-P01
 * 
 **********************************************************************************************/

#include "ql_oe.h"
#include "example_sms.h"

/////////////////////////////////////////////////////////////
// MACRO CONSTANTS DEFINITIONS
///////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////
// ENUM TYPE DEFINITIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// STRUCT TYPE DEFINITIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
/////////////////////////////////////////////////////////////
static int EXSMS_sExit(void);

/////////////////////////////////////////////////////////////
// GLOBAL DATA DEFINITION
/////////////////////////////////////////////////////////////
static sms_client_handle_type h_exsms;


void print_hexstr(uint8_t *apdu,uint32_t apdu_len)
{
    int i = 0;
    char apdu_temp[1024] = {0,};
    char *temp = apdu_temp;
    for(i = 0; i < apdu_len; i++)
    {
        temp +=sprintf(temp,"%02x ",apdu[i]);
    }

    printf("sms pdu_len = %d , pdu = %s \n",apdu_len,apdu_temp);
}

uint8_t chrtodec(char chr)
{
    uint8_t value=0;

    if((chr>='a')&&(chr<='z'))
            chr=chr-32;

    if((chr>='0')&&(chr<='9'))
            value=chr-48;
    else if((chr>='A')&&(chr<='Z'))
            value=chr-65+10;

    return value;
}

int cal16match(uint8_t n)
{
    uint8_t i=0;
    int value=1;

    for(i=0;i<n;i++)
            value*=16;

    return value;
}

int hexstrto_dec(char *chr)
{
    uint8_t i,chrlen;
    uint8_t sta=0; 
    int chrtoint=0;
    int value=0;

    chrlen=strlen(chr);

    if((chr[0] == '0') && ((chr[1]=='x') || (chr[1] == 'X')))
    {
            sta=2;    
            chrlen=chrlen-2;
    }

    for(i=0;i<chrlen;i++)
    {
            chrtoint=chrtodec(chr[sta+i]);
            if(i!=(chrlen-1))             
                    value+=chrtoint*cal16match(chrlen-1-i);
            else if(i==(chrlen-1))        
                    value+=chrtoint;

    }
        return value;
}

bool long_hexstr_to_oct
(
    uint8_t     *pSrc,
    uint32_t    uSrcLen,
    uint8_t     *pDest,
    uint32_t    *pDestLen,
    uint32_t     uDestMaxLen
)
{
   if(pSrc == NULL || pDest == NULL || pDestLen == NULL)
   {
        printf("Paramer error\n");
        return false;
   }

   if(uSrcLen % 2 != 0)
   {
        printf("Paramer error uSrcLen:%d\n",uSrcLen);
        return false;
   }

   uint8_t *pTmp = pDest;
   int i = 0;
   char cStr[3] = {0,};
   if(uDestMaxLen < (uSrcLen / 2))
   {
        printf("Paramer error uDestMaxLen:%d\n",uDestMaxLen);
        return false;
   }

   for(i = 0; i < uSrcLen; i += 2)
   {
        memset(cStr,0x00,sizeof(cStr));
        memcpy(cStr,&pSrc[i],2);
        *pTmp = hexstrto_dec(cStr);
        pTmp++;
   }

   *pDestLen = pTmp - pDest;

   return TRUE;

}


st_exsms_item a_exsms_menu[] = 
{
    {61,    "Send SMS"},
    {62,    "QL_SMS_Send_SmsPdu"},
    {63,    "QL_SMS_GetSmsCenterAddress"},
    {64,    "QL_SMS_SetSmsCenterAddress"},

    //Warning!==>Please add new item upper this line!
    {-1,    NULL}
};

/////////////////////////////////////////////////////////////
// QCERTIOPTION DEFINITIONS ==> Static functions
// NOTE: Static functions ONLY are used internal in this file.
/////////////////////////////////////////////////////////////

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sIsRegisterToNW
 *
 * DESCRIPTION
 *  Check whether register to network or not
 *  
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sIsRegisterToNW(void)
{
    E_QL_NW_RADIO_ACCESS_TYPE_T eRat = E_QL_NW_RAT_GSM;
    E_QL_WWAN_NET_REG_STATE_T eState = E_QL_WWAN_REG_NOT_REGISTERED;
    int iRssi = 0;
    int iBer = 0;
    int iResult = 0;

    iResult = QL_NW_GetRegState(&eRat, &eState, &iRssi, &iBer);
    if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! QL_NW_GetRegState FAIL!",iResult);
        return -1;
    }

    SMS_TRACE("SMS_NOTE: eRat:%d,eState:%d,iRssi:%d,iBer:%d",eRat,eState,iRssi,iBer);

    if(    (eState != E_QL_WWAN_REG_REGISTERED_HOME_NETWORK)
        && (eState != E_QL_WWAN_REG_ROAMING)
      )
    {
        SMS_TRACE("SMS_FAIL! eState:%d NOT register.eRat:%d",eState,eRat);
        return -1;
    }

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbStore
 *
 * DESCRIPTION
 *  Receive a new SMS stored in NV or UIM
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static void EXSMS_sDisplayHelp(void)
{
    int i = 0;

    printf("Supported test cases:\n");
    
    for(i = 0; ; i++)
    {
        if(a_exsms_menu[i].item == -1)
        {
            break;
        }
        
        printf("%d:\t%s\n", a_exsms_menu[i].item, a_exsms_menu[i].name);
    }
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbStore
 *
 * DESCRIPTION
 *  Receive a new SMS stored in NV or UIM
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sRecvCbStore(QL_SMS_MsgRef msgRef)
{
    char *msg_format[] = {"CDMA",  "GW"};
    char *storage_type[] = {"UIM",   "NV"};

    if(NULL == msgRef)
    {
        SMS_TRACE("SMS_FAIL! msgRef is NULL.");
        return -1;
    }

    if(    (E_QL_SMS_MESSAGE_MODE_CDMA != msgRef->e_mode)
        && (E_QL_SMS_MESSAGE_MODE_GW != msgRef->e_mode)
      )
    {
        SMS_TRACE("SMS_FAIL! e_mode:%d is INVALID.",msgRef->e_mode);
        return -1;
    }

    if(    (E_QL_SMS_STORAGE_TYPE_UIM != msgRef->e_storage)
        && (E_QL_SMS_STORAGE_TYPE_NV != msgRef->e_storage)
      )
    {
        SMS_TRACE("SMS_FAIL! e_storage:%d is INVALID.",msgRef->e_storage);
        return -1;
    }

    QL_USER_LOG(
        "###You've got one new %s message, stored to %s index=%d\n", 
        msg_format[msgRef->e_mode],
        storage_type[msgRef->e_storage],
        msgRef->storage_index
    );  

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sHexStrToHexOct
 *
 * DESCRIPTION
 *  Convert HEX-STR to HEX-OCT, like "3132" -> 0x31 0x32
 *  
 * PARAMETERS
 *  <dst>  [OUT] The pointer of destination buffer
 *  <hex>     [IN] The pointer of hex string
 *  <dst_len> [IN] The maximum length of destination buffer
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sHexStrToHexOct(unsigned char *dst, const char *hex, size_t dst_len)
{
    int si = 0;
    int di = 0;
    unsigned char o = 0;
    int c = 0;
   
    if((NULL == dst) || (NULL == hex) || (0 == dst_len))
    {
        SMS_TRACE("SMS_FAIL! Paramter is INVALID. dst:%p,hex:%p,dst_len:%d",dst,hex,dst_len);
        return -1;
    }

    for (si = 0, di = 0; di < (dst_len)*2 && hex[si]; si ++ )
    {
        c = (int)hex[si];

        if (isspace(c)) 
        {
            continue;
        }

        if (!isxdigit(c)) 
        {
            return -1;
        }

        c = tolower(c);
        c = c >= 'a' ? 10 + c - 'a' : c - '0';
        o = o << 4;
        o |= ((char)c) & 0x0f;
        if (di % 2 != 0)
        {
            dst[di/2] = o & 0xff;
        }
        di ++;
    }

    return di / 2;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbDisplay
 *
 * DESCRIPTION
 *  Receive a new SMS(UCS2) only for display(NOT store)
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
int EXSMS_sRecvCbDisplayUcs2(QL_SMS_MsgRef msgRef)
{
    int i = 0;
    unsigned char smsbuf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};

   
 
    if(msgRef->user_data_head_valid)
    {
            QL_USER_LOG(
                            "\n###You've got one new UCS2 Long msg from %s at %s    total_segments:%d   seg_number:%d  len=%d content=\n", 
                            msgRef->src_addr,
                            msgRef->timestamp,
                            msgRef->user_data_head.total_segments,
                            msgRef->user_data_head.seg_number,
                            msgRef->sms_data_len
                       );
    }
    else
    {
            QL_USER_LOG(
                            "\n###You've got one new UCS2 msg from %s at %s, len=%d, content=%s\n", 
                            msgRef->src_addr,
                            msgRef->timestamp,
                            msgRef->sms_data_len,
                            smsbuf
    );
    }   
    printf("Received UCS raw data:");

    for(i = 0; i < msgRef->sms_data_len; i++)
    {
        printf("%.2X", msgRef->sms_data[i]);
    }

    printf("\n");

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbDisplay
 *
 * DESCRIPTION
 *  Receive a new SMS(BIN-DATA) only for display(NOT store)
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sRecvCbDisplayBin(QL_SMS_MsgRef msgRef)
{
    int i = 0;

    if(msgRef->user_data_head_valid)
    {
            QL_USER_LOG(
                            "###You've got one new BINARY Long msg from %s at %s    total_segments:%d   seg_number:%d  len=%d  content=\n", 
                            msgRef->src_addr,
                            msgRef->timestamp,
                            msgRef->user_data_head.total_segments,
                            msgRef->user_data_head.seg_number,
                            msgRef->sms_data_len
                       );
    }
    else
    {
             QL_USER_LOG(
                             "###You've got one new BINARY msg from %s at %s , len=%d, content=", 
                             msgRef->src_addr,
                             msgRef->timestamp,
                             msgRef->sms_data_len
                        );
   
    }
    
    for(i = 0; i < msgRef->sms_data_len; i++)
    {
        printf("%.2X", msgRef->sms_data[i]);
    }
    printf("\n");

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbDisplay
 *
 * DESCRIPTION
 *  Receive a new SMS(GSM-7bit) only for display(NOT store)
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sRecvCbDisplayGsm(QL_SMS_MsgRef msgRef)
{
    int i = 0;
    
    if(msgRef->user_data_head_valid)
    {
            QL_USER_LOG(
                            "###You've got one new GSM-7 Long msg from %s at %s    total_segments:%d   seg_number:%d len:%d  content=%s\n", 
                            msgRef->src_addr,
                            msgRef->timestamp,
                            msgRef->user_data_head.total_segments,
                            msgRef->user_data_head.seg_number,
                            msgRef->sms_data_len,
                            msgRef->sms_data
                       );
    }
    else
    {
             QL_USER_LOG(
                            "###You've got one new GSM-7 msg from %s at %s content=%s\n", 
                            msgRef->src_addr,
                            msgRef->timestamp,
                            msgRef->sms_data
                       );
   
    }
    
    for(i = 0; i < msgRef->sms_data_len; i++)
    {
        printf("%.2X", msgRef->sms_data[i]);
    }
    
    printf("\n\n");

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbDisplay
 *
 * DESCRIPTION
 *  Receive a new SMS only for display(NOT store)
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sRecvCbDisplay(QL_SMS_MsgRef msgRef)
{
    int iResult = 0;

    if(NULL == msgRef)
    {
        SMS_TRACE("SMS_FAIL! msgRef is NULL.");
        return -1;
    }

    if(msgRef->format == E_QL_SMS_FORMAT_UCS2)
    {
        iResult = EXSMS_sRecvCbDisplayUcs2(msgRef);
    }
    else if(msgRef->format == E_QL_SMS_FORMAT_BINARY_DATA)
    {
        iResult = EXSMS_sRecvCbDisplayBin(msgRef);
    }
    else
    {
        iResult = EXSMS_sRecvCbDisplayGsm(msgRef);
    }

    return iResult;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRecvCbDisplay
 *
 * DESCRIPTION
 *  Callback handler of receive a new SMS only
 *  
 * PARAMETERS
 *  <msgRef>  [IN] The 'QL_SMS_MsgRef' data
 *  <contextPtr>  [IN] The pointer of context
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static void EXSMS_sRecvCb( QL_SMS_MsgRef msgRef, void* contextPtr)
{
    int i = 0;

    if(NULL == msgRef)
    {
        SMS_TRACE("SMS_FAIL! msgRef is NULL.");
        return;
    }
    
    if(msgRef->e_storage != E_QL_SMS_STORAGE_TYPE_NONE)
    {
        EXSMS_sRecvCbStore(msgRef);
    }
    else
    {
        EXSMS_sRecvCbDisplay(msgRef);
    }

    return;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSendGetDataUcs2
 *
 * DESCRIPTION
 *  Get UCS2 data of SMS from stdin
 *  
 * PARAMETERS
 *  <pInfo>  [OUT] The pointer of 'ql_sms_info_t' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSendGetDataUcs2(ql_sms_info_t *pInfo)
{
    char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
    int iBufLen = 0;
    int len = 0;

    printf("please input UCS2 data: \n");

    scanf("%s", sms_buf);

    iBufLen = strlen(sms_buf);
    if(iBufLen % 4)
    {
        SMS_TRACE("SMS_FAIL! iBufLen:%d % 4 NOT 0.",iBufLen);
        return -1;
    }

    len = EXSMS_sHexStrToHexOct(pInfo->sms_data,sms_buf,sizeof(pInfo->sms_data));
    if(-1 == len)
    {
        SMS_TRACE("SMS_FAIL! EXSMS_sHexStrToHexOct FAIL! sms_buf:%s.",sms_buf);
        return -1;
    }

    pInfo->sms_data_len = len;

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSendGetDataBin
 *
 * DESCRIPTION
 *  Get binary data of SMS from stdin
 *  
 * PARAMETERS
 *  <pInfo>  [OUT] The pointer of 'ql_sms_info_t' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSendGetDataBin(ql_sms_info_t *pInfo)
{
    char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0,};
    int iBufLen = 0;
    int len = 0;
    
    printf("please input binary data: \n");

    scanf("%s", sms_buf);

    iBufLen = strlen(sms_buf);
    if(iBufLen % 2)
    {
        SMS_TRACE("SMS_FAIL! iBufLen:%d % 2 NOT 0.",iBufLen);
        return -1;
    }

    len = EXSMS_sHexStrToHexOct(pInfo->sms_data,sms_buf,sizeof(pInfo->sms_data));
    if(-1 == len)
    {
        SMS_TRACE("SMS_FAIL! EXSMS_sHexStrToHexOct FAIL! sms_buf:%s.",sms_buf);
        return -1;
    }

    pInfo->sms_data_len = len;

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSendGetDataGsm
 *
 * DESCRIPTION
 *  Get GSM data of SMS from stdin
 *  
 * PARAMETERS
 *  <pInfo>  [OUT] The pointer of 'ql_sms_info_t' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSendGetDataGsm(ql_sms_info_t *pInfo)
{

 	char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH+1] = {0,};
 	char c;
 	int iBufLen = 0;
 
 	printf("please input GSM data: \n");
 
 	while ((c = getchar()) != EOF && c != '\n');//Add this on purpose or the following fgets will be skipped.
 	fgets(sms_buf, sizeof(sms_buf), stdin);
 	iBufLen = strlen(sms_buf); //to skip 0x0A
 
 	if(iBufLen <= QL_SMS_MAX_MT_MSG_LENGTH && sms_buf[iBufLen-1] == 0x0A)
 	{
 		sms_buf[iBufLen-1] = '\0';
 	}
 
 	memcpy(pInfo->sms_data, sms_buf,QL_SMS_MAX_MT_MSG_LENGTH);
 	pInfo->sms_data_len = strlen(sms_buf);
 
	return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSendGetDataIra
 *
 * DESCRIPTION
 *  Get IRA data of SMS from stdin
 *  
 * PARAMETERS
 *  <pInfo>  [OUT] The pointer of 'ql_sms_info_t' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSendGetDataIra(ql_sms_info_t *pInfo)
{
    char sms_buf[QL_SMS_MAX_MT_MSG_LENGTH] = {0};
    int iBufLen = 0;

    printf("please input IRA data: \n");

    scanf("%s", sms_buf);

    iBufLen = strlen(sms_buf);

    strcpy(pInfo->sms_data, sms_buf);
    pInfo->sms_data_len = iBufLen;

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSendGetData
 *
 * DESCRIPTION
 *  Get data of SMS from stdin
 *  
 * PARAMETERS
 *  <e_format>  [IN] 'E_QL_SMS_FORMAT_T' data
 *  <pInfo>  [OUT] The pointer of 'ql_sms_info_t' data
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSendGetData(E_QL_SMS_FORMAT_T e_format,ql_sms_info_t *pInfo)
{
    int iResult = -1; //Warning! MUST initialize to -1

    if(NULL == pInfo)
    {
        SMS_TRACE("SMS_FAIL! pInfo is NULL.");
        return -1;
    }

    if(E_QL_SMS_FORMAT_GSM_7BIT == e_format)
    {
        iResult = EXSMS_sSendGetDataGsm(pInfo);
    }
    else if(E_QL_SMS_FORMAT_UCS2 == e_format)
    {
        iResult = EXSMS_sSendGetDataUcs2(pInfo);
    }
    else if(E_QL_SMS_FORMAT_BINARY_DATA == e_format)
    {
        iResult = EXSMS_sSendGetDataBin(pInfo);
    }
    else if(E_QL_SMS_FORMAT_IRA == e_format)
    {
        iResult = EXSMS_sSendGetDataIra(pInfo);
    }
    else
    {
        SMS_TRACE("SMS_FAIL! e_format:%d is INVALID.",e_format);
    }

    return iResult;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSend
 *
 * DESCRIPTION
 *  The handler for send SMS
 *  
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  -1, FAIL!
 *  0, SUCC
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSend(void)
{
    E_QL_SMS_FORMAT_T   e_format= 0;
    ql_sms_info_t       t_sms_info = {0};
    int iResult = 0;

    iResult = EXSMS_sIsRegisterToNW();
    if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! EXSMS_sIsRegisterToNW FAIL!");
        return -1;
    }

    printf("please input dest phone number: \n");
    scanf("%s", t_sms_info.src_addr);

    printf("please input sms encoding type(0:GSM-7, 1:Binary, 2:UCS2, 3:IRA): \n");
    scanf("%d", &e_format);

    iResult = EXSMS_sSendGetData(e_format,&t_sms_info);
    if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! EXSMS_sSendGetData FAIL! e_format:%d.",e_format);
        return -1;
    }

    t_sms_info.format = e_format;
               
    iResult = QL_SMS_Send_Sms(h_exsms, &t_sms_info);
    if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! QL_SMS_Send_Sms FAIL! iResult:%d.",iResult);
        return -1;
    }
    
    printf("#QL_SMS_Send_Sms succ \n");

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSigHandlerSIGINT
 *
 * DESCRIPTION
 *  The handler for signal:SIGINT used by EXSMS
 *  
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sSigHandlerSIGINT(void)
{
    EXSMS_sExit();

    exit(-1);

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sSigHandler
 *
 * DESCRIPTION
 *  The handler for signal used by EXSMS
 *  
 * PARAMETERS
 *  <sig>  [IN] signal value
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static void EXSMS_sSigHandler(int sig)
{
    printf("######## got signal %d #######\n", sig);

    if(SIGINT == sig)
    {
        EXSMS_sSigHandlerSIGINT();
    }
    else
    {
        SMS_TRACE("SMS_WARNING! sig:%d NOT handle.",sig);
    }

    return;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sRegisterSigs
 *
 * DESCRIPTION
 *  Register signals used by EXSMS
 *  
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static void EXSMS_sRegisterSigs(void)
{
    signal(SIGINT, EXSMS_sSigHandler);

    return;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sInit
 *
 * DESCRIPTION
 *  Initialize for EXSMS
 *  
 * PARAMETERS
 *  -1, FAIL!
 *  0, SUCC
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sInit(void)
{
    int iResult = 0;

    //Initialize
    h_exsms = 0;

    EXSMS_sRegisterSigs();

	iResult = QL_SMS_Client_Init(&h_exsms);
	if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! QL_SMS_Client_Init FAIL! iResult:%d",iResult);
        return -1;
    }

    //Register receive SMS callback handler
    iResult = QL_SMS_AddRxMsgHandler(EXSMS_sRecvCb, (void*)h_exsms);
    if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! QL_SMS_AddRxMsgHandler FAIL! iResult:%d",iResult);
    }

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  EXSMS_sExit
 *
 * DESCRIPTION
 *  Exit process for EXSMS
 *  
 * PARAMETERS
 *  -1, FAIL!
 *  0, SUCC
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  
 *****************************************************************************/
static int EXSMS_sExit(void)
{
    int iResult = 0;

    if(0 == h_exsms)
    {
        SMS_TRACE("SMS_WARNING! h_exsms NOT initialize.");
        return 0;
    }

    iResult = QL_SMS_Client_Deinit(h_exsms);
	if(iResult != 0)
    {
        SMS_TRACE("SMS_FAIL! QL_SMS_Client_Deinit FAIL! iResult:%d",iResult);
        return -1;
    }

    h_exsms = 0;

    return 0;
}

/////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS ==> Extern functions
/////////////////////////////////////////////////////////////
/*****************************************************************************
 * FUNCTION
 *  main
 *
 * DESCRIPTION
 *  The main entry for EXSMS
 *  
 * PARAMETERS
 *  <argc>  [IN] The count of arguments of command input
 *  <argv>  [IN] The value of arguments of command input
 *
 * RETURNS
 *  0: SUCC
 *  OTHER VALUES: FAIL
 *
 * NOTE
 *  
 *****************************************************************************/
int main(int argc, char *argv[])
{
    int item = 0;
    int iResult = 0;

    EXSMS_sInit();

    while(1)
    {
        EXSMS_sDisplayHelp();
        
        printf("please input item(-1 exit): ");

        scanf("%d", &item);
        
        switch(item)
        {
            case 61://"Send SMS"
            {
                EXSMS_sSend();
            }
            break;
            case 62:
            {
                char aPduMsg[255*2+1]={0};
                ql_wms_send_raw_message_data_t sWmsRawMsg;
                ql_wms_raw_send_resp_t  sWmsRawResp;

                memset(&sWmsRawMsg,0,sizeof(sWmsRawMsg));
                memset(&sWmsRawResp,0,sizeof(sWmsRawResp));

                printf("please input PDU content: \n");
                scanf("%s",aPduMsg);
                if(false == long_hexstr_to_oct(aPduMsg,strlen(aPduMsg),sWmsRawMsg.raw_message,&sWmsRawMsg.raw_message_len,sizeof(sWmsRawMsg.raw_message)))
                {
                    printf(" long_hexstr_to_oct FAIL\n");
                    return -1;
                }

                print_hexstr(sWmsRawMsg.raw_message,sWmsRawMsg.raw_message_len);

                printf("please input PDU FORMAT(0-CAMD_PP; 6-GW_PP): \n");
                uint8_t uFormat = 0;
                scanf("%d",&uFormat);
                sWmsRawMsg.format = uFormat;

                printf("please FORMAT = %d \n",sWmsRawMsg.format);

                iResult = QL_SMS_Send_SmsPdu(h_exsms,sWmsRawMsg,&sWmsRawResp);
                if(iResult < 0)
                {
                    printf("QL_SMS_Send_SmsPdu FAIL. iResult:%d\n",iResult);
                }
                else
                {
                    printf("  message_id:%d\n",sWmsRawResp.message_id);
                }

                if(sWmsRawResp.cause_code_valid)
                {
                   printf(" cause_code:%02x\n",sWmsRawResp.cause_code); 
                }

            }
            break;                


            case 63:
            {
               ql_sms_service_center_cfg_t get_sca_cfg;
               memset(&get_sca_cfg,0,sizeof(get_sca_cfg));

               iResult = QL_SMS_GetSmsCenterAddress(h_exsms,&get_sca_cfg);

               if(iResult < 0)
               {
                   printf("QL_SMS_GetSmsCenterAddress. iResult:%d\n",iResult);
               }
               else
               {
                   printf(" ervice_center_addr:%s   service_center_addr_type:%s \n",
                            get_sca_cfg.service_center_addr,get_sca_cfg.service_center_addr_type);
               }
               break;
            }

            case 64:
            {
               ql_sms_service_center_cfg_t set_sca_cfg;
               memset(&set_sca_cfg,0,sizeof(set_sca_cfg));

               printf("please input service center addr: \n");
               scanf("%s",set_sca_cfg.service_center_addr);

               printf("please input service center addr type(\"145\"-with'+';  \"129\"--whithout '+'): \n");
               scanf("%s",set_sca_cfg.service_center_addr_type);
               set_sca_cfg.service_center_addr_type_valid = true;
               iResult = QL_SMS_SetSmsCenterAddress(h_exsms,&set_sca_cfg);

               if(iResult != 0)
               {
                   printf("QL_SMS_SetSmsCenterAddress. iResult:%d\n",iResult);
               }

               break;
            }

            default:
            {
            }
            break;   
        }

        if(-1 == item)
        {
            break;
        }
    }

    EXSMS_sExit();
    
    return 0;
}
