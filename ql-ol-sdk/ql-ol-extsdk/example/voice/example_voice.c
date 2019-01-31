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
    {1,   "QL_Voice_Call_Start"},
    {2,   "QL_Voice_Call_End"},
    {3,   "QL_Voice_Call_Answer"},
    {4,   "QL_Voice_Call_CancelDial"},

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


int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char   phoneNum[32] = {0};
    voice_client_handle_type    h_voice     = 0;
    int                         voice_call_id = 0;

    ret = QL_Voice_Call_Client_Init(&h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_Client_Init FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);

    ret = QL_Voice_Call_AddStateHandler(h_voice, ql_voice_call_cb_func, &voice_call_id);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_AddStateHandler FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_AddStateHandler ret = %d\n", ret);

    print_help();

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
        case 1://"QL_Voice_Call_Start"
        {
            char    PhoneNum[32]                  = {0}; 

            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);
            
            ret = QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum, &voice_call_id);
            printf("QL_Voice_Call_Start ret = %d, with voice_call_id=%d\n", ret, voice_call_id);
            break;
        }
        case 2://"QL_Voice_Call_End"
        {
            ret = QL_Voice_Call_End(h_voice, voice_call_id);
            printf(" ret = %d\n", ret);
            break;
        }
        case 3://"QL_Voice_Call_Answer"
        {
            
            printf(" voice_call_id = %d\n", voice_call_id);
            ret = QL_Voice_Call_Answer(h_voice, voice_call_id);
            printf(" ret = %d\n", ret);
            break;
        }

        case 4://QL_Voice_Call_CancelDial
        {
            ret = QL_Voice_Call_CancelDial(h_voice);
            printf(" QL_Voice_Call_CancelDial ret = %d\n", ret);
            break;
        }

       default:
            print_help();
            break;   
        }
    }

    ret = QL_Voice_Call_RemoveStateHandler(h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_RemoveStateHandler FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_RemoveStateHandler ret = %d\n", ret);


    ret = QL_Voice_Call_Client_Deinit(h_voice);
    if(ret != 0 && ret != 1)
    {
        printf("QL_Voice_Call_Client_Deinit FAIL.%d\n");
        return -1;
    }
    printf("QL_Voice_Call_Client_Deinit ret = %d, with h_voice=%d\n", ret, h_voice);

    return 0;
}
