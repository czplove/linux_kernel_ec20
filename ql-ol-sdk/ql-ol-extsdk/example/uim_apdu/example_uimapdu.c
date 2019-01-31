#include <ql_oe.h>

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;
    
//for server test
st_api_test_case at_api_testcases[] = 
{
    {0,   "print_help"},
    {1,   "QL_SIM_OpenLogicalChannel"},
    {2,   "QL_SIM_SendApdu"},
    {3,   "QL_SIM_CloseLogicalChannel"},
    
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


void print_hexstr(uint8_t *apdu,uint32_t apdu_len)
{
    int i = 0;
    char apdu_temp[1024] = {0,};
    char *temp = apdu_temp;
    for(i = 0; i < apdu_len; i++)
    {
        temp +=sprintf(temp,"%02x ",apdu[i]);
    }

    printf("apdu_len = %d , apdu = %s \n",apdu_len,apdu_temp);
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

int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    uint8_t channel_id = 0;

    ST_QL_UIM_AID_INFO_TYPE sAidInfo;

    print_help();

    char aid[32+1] = {0,};
    printf("please input aid(test:A0000000871002FF86FFFF89FFFFFFFF): \n");
    scanf("%s", aid);
    if(false == long_hexstr_to_oct(aid,strlen(aid),sAidInfo.aid,&sAidInfo.aid_len,sizeof(sAidInfo.aid)))
    {
        printf(" long_hexstr_to_oct FAIL\n");
        return -1;
    }

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
            case 0://"print_help"
            {
                print_help();
                break;
            }
            case 1://"QL_SIM_OpenLogicalChannel"
            {
                ST_QL_UIM_LOGCAL_CHANNEL_TYPE sOpenLogcalChannelResp;

                ret = QL_SIM_OpenLogicalChannel(E_QL_UIM_SLOT_1,sAidInfo,&sOpenLogcalChannelResp);
                if(ret < 0)
                {
                    printf("QL_SIM_OpenLogicalChannel FAIL.	ret:%d\n",ret);
                    return -1;
                }

                printf("SW1:%02x, SW2:%02x\n",sOpenLogcalChannelResp.card_result.sw1,sOpenLogcalChannelResp.card_result.sw2);
                if(sOpenLogcalChannelResp.channel_id_valid)
                {
                    channel_id = sOpenLogcalChannelResp.channel_id;
                }
                else
                {
                    printf("Get channel_id FAIL.\n");
                    return -1;
                }

                if(sOpenLogcalChannelResp.select_response_valid)
                {
                    print_hexstr(sOpenLogcalChannelResp.select_response,sOpenLogcalChannelResp.select_response_len);
                }

                break;
            }
            case 2://"QL_SIM_SendApdu"
            {
                ST_QL_UIM_SEND_APDU_TYPE sApdu;
                ST_UIM_SEND_APDU_RESPONSE_TYPE sSendApduResp;

                char aApdu[512] = {0,};//"00A40804022FE2";
                printf("please input apdu(test:00A40804022FE2): \n");
                scanf("%s", aApdu);

                memset(&sApdu,0,sizeof(sApdu));
                memset(&sSendApduResp,0,sizeof(sSendApduResp));

                if(false == long_hexstr_to_oct(aApdu,strlen(aApdu),sApdu.apdu,&sApdu.apdu_len,sizeof(sApdu.apdu)))
                {
                    printf(" long_hexstr_to_oct FAIL\n");
                    return -1; 
                }

                sApdu.channel_id = channel_id;

                ret = QL_SIM_SendApdu(E_QL_UIM_SLOT_1, sApdu, &sSendApduResp);
                if(ret < 0)
                {
                    printf("QL_SIM_SendApdu FAIL.		ret:%d\n",ret);
                    return -1;
                }
                printf("QL_SIM_SendApdu ret:%d\n",ret);

                if(sSendApduResp.apdu_valid)
                {
                    print_hexstr(sSendApduResp.apdu,sSendApduResp.apdu_len);
                }

                break;
            }

              case 3:
              {
                  //laurence 20180402, update api error 
                  ret = QL_SIM_CloseLogicalChannel(E_QL_UIM_SLOT_1,channel_id);
                  if(ret < 0)
                  {
                      printf("QL_SIM_CloseLogicalChannel FAIL.	ret:%d\n",ret);
                      return -1;
                  }
              }

              default:
              print_help();
                    break;   
        }
    }

    
    return 0;
}
